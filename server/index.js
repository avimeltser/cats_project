import cors from "cors";
import dotenv from "dotenv";
import express from "express";
import rateLimit from "express-rate-limit";
import crypto from "crypto";
import compression from "compression";
import helmet from "helmet";
import path from "path";
import pino from "pino";
import pinoHttp from "pino-http";
import { fileURLToPath } from "url";
import { z } from "zod";

dotenv.config();

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const isProd = process.env.NODE_ENV === "production";
const logger = pino({
  level: process.env.LOG_LEVEL || (isProd ? "info" : "debug")
});

const requireEnv = (name) => {
  const value = process.env[name];
  if (!value) {
    const message = `Missing required env var: ${name}`;
    if (isProd) {
      console.error(message);
      process.exit(1);
    } else {
      console.warn(message);
    }
  }
  return value;
};

const app = express();
const PORT = Number(process.env.PORT) || 3001;
const CLIENT_ORIGIN = process.env.CORS_ORIGIN || "http://localhost:4173";
const ALLOWED_ORIGINS = CLIENT_ORIGIN.split(",").map((origin) => origin.trim());

app.use(
  helmet({
    crossOriginResourcePolicy: { policy: "cross-origin" }
  })
);
app.use(compression());
app.use(
  cors({
    origin: (origin, callback) => {
      if (!origin || ALLOWED_ORIGINS.includes(origin)) {
        callback(null, true);
        return;
      }
      callback(new Error("Not allowed by CORS"));
    }
  })
);
app.use(express.json({ limit: "1mb" }));
app.use(
  pinoHttp({
    logger,
    genReqId: () => crypto.randomUUID()
  })
);
app.use((req, res, next) => {
  req.id = req.id || crypto.randomUUID();
  res.setHeader("X-Request-Id", req.id);
  next();
});
app.use(
  rateLimit({
    windowMs: 60 * 1000,
    max: 120,
    standardHeaders: true,
    legacyHeaders: false
  })
);

const AMADEUS_BASE_URL =
  process.env.AMADEUS_ENV === "production"
    ? "https://api.amadeus.com"
    : "https://test.api.amadeus.com";
const AMADEUS_CLIENT_ID = requireEnv("AMADEUS_CLIENT_ID");
const AMADEUS_CLIENT_SECRET = requireEnv("AMADEUS_CLIENT_SECRET");

let amadeusToken = null;
let amadeusTokenExpiresAt = 0;
let amadeusTokenError = null;
let amadeusLastError = null;

const cacheStore = new Map();

const fetchWithTimeout = async (url, options = {}, timeoutMs = 8000) => {
  const controller = new AbortController();
  const timeout = setTimeout(() => controller.abort(), timeoutMs);
  try {
    return await fetch(url, { ...options, signal: controller.signal });
  } finally {
    clearTimeout(timeout);
  }
};

const getCache = (key) => {
  const cached = cacheStore.get(key);
  if (!cached) return null;
  if (cached.expiresAt < Date.now()) {
    cacheStore.delete(key);
    return null;
  }
  return cached.value;
};

const setCache = (key, value, ttlMs) => {
  cacheStore.set(key, { value, expiresAt: Date.now() + ttlMs });
};

const fallbackRequest = {
  origin: "NYC",
  destination: "Lisbon",
  startDate: "2026-03-10",
  endDate: "2026-03-20",
  travelers: 2,
  budget: 3200,
  interests: "food,culture,coast"
};

const diffNights = (startDate, endDate) => {
  const start = new Date(startDate);
  const end = new Date(endDate);
  const diff = Math.round((end - start) / (1000 * 60 * 60 * 24));
  return Number.isFinite(diff) && diff > 0 ? diff : 1;
};

const buildQuote = (request, liveResults) => {
  const flightTotal = liveResults?.flight?.total ?? 1340;
  const stayTotal = liveResults?.stay?.total ?? 920;
  const transportTotal = 220;
  const activityTotal = 310;
  const fee = 89;

  return {
    requestId: `tw_${Date.now()}`,
    currency: liveResults?.currency ?? "USD",
    request,
    providers: [
      {
        type: "flights",
        name: "Amadeus",
        status: liveResults?.flight ? "live" : "mock"
      },
      {
        type: "stays",
        name: "Amadeus",
        status: liveResults?.stay ? "live" : "mock"
      },
      { type: "transport", name: "CityHop", status: "mock" },
      { type: "activities", name: "LocalMuse", status: "mock" }
    ],
    itinerary: {
      flights: [
        {
          segment: liveResults?.flight?.segment ?? `${request.origin} → ${request.destination}`,
          cabin: "Economy Flex",
          carrier: liveResults?.flight?.carrier ?? "SkyRoutes",
          carrierCode: liveResults?.flight?.carrierCode ?? null,
          departure: liveResults?.flight?.departure ?? null,
          arrival: liveResults?.flight?.arrival ?? null,
          duration: liveResults?.flight?.duration ?? null,
          stops: liveResults?.flight?.stops ?? null,
          total: flightTotal
        }
      ],
      stays: [
        {
          name: liveResults?.stay?.name ?? "Riverside Boutique Hotel",
          nights: liveResults?.stay?.nights ?? diffNights(request.startDate, request.endDate),
          rating: liveResults?.stay?.rating ?? 4.6,
          address: liveResults?.stay?.address ?? null,
          total: stayTotal
        }
      ],
      transport: [
        {
          mode: "Airport transfer + 3-day rail pass",
          total: transportTotal
        }
      ],
      activities: [
        {
          name: "Chef-led food tour",
          total: 180
        },
        {
          name: "Coastal day trip",
          total: 130
        }
      ]
    },
    totals: {
      base: flightTotal + stayTotal + transportTotal + activityTotal,
      fee,
      total: flightTotal + stayTotal + transportTotal + activityTotal + fee
    },
    notes: [
      ...(liveResults?.notes ?? []),
      "All totals are estimates and exclude taxes."
    ],
    debug: liveResults?.debug ?? null,
    stayCandidates: liveResults?.stayCandidates ?? []
  };
};

app.get("/api/health", (_req, res) => {
  res.json({
    status: "ok",
    timestamp: new Date().toISOString(),
    uptime: process.uptime(),
    env: process.env.NODE_ENV || "development"
  });
});

const getAmadeusToken = async () => {
  if (!AMADEUS_CLIENT_ID || !AMADEUS_CLIENT_SECRET) {
    return null;
  }

  const now = Date.now();
  if (amadeusToken && now < amadeusTokenExpiresAt) {
    return amadeusToken;
  }

  const body = new URLSearchParams({
    grant_type: "client_credentials",
    client_id: AMADEUS_CLIENT_ID,
    client_secret: AMADEUS_CLIENT_SECRET
  });

  let response;
  try {
    response = await fetchWithTimeout(`${AMADEUS_BASE_URL}/v1/security/oauth2/token`, {
      method: "POST",
      headers: { "Content-Type": "application/x-www-form-urlencoded" },
      body
    });
  } catch (error) {
    amadeusTokenError = {
      status: "FETCH_FAILED",
      body: error?.message?.slice(0, 600) || "Token request failed"
    };
    return null;
  }

  if (!response.ok) {
    const text = await response.text();
    amadeusTokenError = {
      status: response.status,
      body: text.slice(0, 600)
    };
    return null;
  }

  const data = await response.json();
  amadeusToken = data.access_token;
  amadeusTokenExpiresAt = now + (data.expires_in ?? 0) * 1000 - 60_000;
  amadeusTokenError = null;

  return amadeusToken;
};

const amadeusGet = async (path, params, options = {}) => {
  const cacheKey = options.cacheKey || `${path}?${new URLSearchParams(params).toString()}`;
  const ttlMs = options.ttlMs ?? 0;
  if (ttlMs > 0) {
    const cached = getCache(cacheKey);
    if (cached) return cached;
  }

  const token = await getAmadeusToken();
  if (!token) {
    return null;
  }

  const url = new URL(`${AMADEUS_BASE_URL}${path}`);
  Object.entries(params).forEach(([key, value]) => {
    if (value !== undefined && value !== null && value !== "") {
      url.searchParams.set(key, String(value));
    }
  });

  let response;
  try {
    response = await fetchWithTimeout(url, {
      headers: { Authorization: `Bearer ${token}` }
    });
  } catch (error) {
    amadeusLastError = {
      status: "FETCH_FAILED",
      path,
      body: error?.message?.slice(0, 600) || "Request failed"
    };
    return null;
  }

  if (!response.ok) {
    const text = await response.text();
    amadeusLastError = {
      status: response.status,
      path,
      body: text.slice(0, 600)
    };
    return null;
  }

  amadeusLastError = null;
  const json = await response.json();
  if (ttlMs > 0) {
    setCache(cacheKey, json, ttlMs);
  }
  return json;
};

const searchSchema = z
  .object({
    origin: z.string().min(1),
    destination: z.string().min(1),
    startDate: z.string().min(1),
    endDate: z.string().min(1),
    travelers: z.coerce.number().int().min(1).max(9),
    budget: z.coerce.number().optional(),
    interests: z.string().optional(),
    bookingMode: z.enum(["autopilot", "copilot"]).optional(),
    hotelId: z.string().optional()
  })
  .refine((data) => !Number.isNaN(Date.parse(data.startDate)), {
    path: ["startDate"],
    message: "Start date is invalid."
  })
  .refine((data) => !Number.isNaN(Date.parse(data.endDate)), {
    path: ["endDate"],
    message: "End date is invalid."
  })
  .refine((data) => new Date(data.endDate) > new Date(data.startDate), {
    path: ["endDate"],
    message: "End date must be after start date."
  });

const validateSearchRequest = (payload) => {
  const parsed = searchSchema.safeParse(payload);
  if (parsed.success) {
    return { errors: null, value: parsed.data };
  }
  const errors = parsed.error.issues.map((issue) => issue.message);
  return { errors, value: null };
};

const resolveIataCode = async (value) => {
  if (!value) return null;
  const trimmed = value.trim();
  if (trimmed.length === 3) {
    return trimmed.toUpperCase();
  }

  const data = await amadeusGet(
    "/v1/reference-data/locations",
    {
      keyword: trimmed,
      subType: "CITY"
    },
    { ttlMs: 6 * 60 * 60 * 1000 }
  );

  return data?.data?.[0]?.iataCode ?? null;
};

const searchFlights = async ({ origin, destination, startDate, adults }) => {
  const originCode = await resolveIataCode(origin);
  const destinationCode = await resolveIataCode(destination);

  if (!originCode || !destinationCode) {
    return null;
  }

  const data = await amadeusGet(
    "/v2/shopping/flight-offers",
    {
      originLocationCode: originCode,
      destinationLocationCode: destinationCode,
      departureDate: startDate,
      adults,
      max: 1
    },
    { ttlMs: 2 * 60 * 1000 }
  );

  const offer = data?.data?.[0];
  const itinerary = offer?.itineraries?.[0];
  const segments = itinerary?.segments ?? [];
  const firstSegment = segments[0];
  const lastSegment = segments[segments.length - 1];
  const total = Number.parseFloat(offer?.price?.total);

  if (!offer || Number.isNaN(total)) {
    return null;
  }

  return {
    segment: `${originCode} → ${destinationCode}`,
    carrier: offer?.validatingAirlineCodes?.[0] ?? "Amadeus",
    carrierCode: offer?.validatingAirlineCodes?.[0] ?? null,
    departure: firstSegment?.departure?.at ?? null,
    arrival: lastSegment?.arrival?.at ?? null,
    duration: itinerary?.duration ?? null,
    stops: Math.max(segments.length - 1, 0),
    total,
    currency: offer?.price?.currency
  };
};

const searchHotels = async ({ destination, startDate, endDate, adults, hotelId }) => {
  const cityCode = await resolveIataCode(destination);
  if (!cityCode) {
    return null;
  }

  const hotelList = await amadeusGet(
    "/v1/reference-data/locations/hotels/by-city",
    {
      cityCode,
      radius: 30,
      radiusUnit: "KM",
      hotelSource: "ALL"
    },
    { ttlMs: 6 * 60 * 60 * 1000 }
  );

  const hotelIds = hotelList?.data?.map((hotel) => hotel.hotelId).filter(Boolean) ?? [];
  const useMockRatings = process.env.ENABLE_REAL_RATINGS !== "true";
  const candidates = (hotelList?.data ?? []).slice(0, 8).map((hotel) => {
    const seed = Array.from(hotel.hotelId || "")
      .reduce((sum, char) => sum + char.charCodeAt(0), 0);
    const reviewCount = useMockRatings ? 80 + (seed % 620) : null;
    const mockRating = useMockRatings ? (3.6 + (seed % 14) * 0.1).toFixed(1) : null;
    return {
      hotelId: hotel.hotelId,
      name: hotel.name,
      rating: useMockRatings ? Number(mockRating) : (hotel.rating ?? null),
      reviewCount,
      ratingSource: useMockRatings ? "mock" : "provider",
      address: [hotel.address?.lines?.join(", "), hotel.address?.cityName]
        .filter(Boolean)
        .join(", ")
    };
  });

  let offers = null;
  const idsToTry = hotelId ? [hotelId] : hotelIds.slice(0, 10);
  for (const id of idsToTry) {
    offers = await amadeusGet(
      "/v3/shopping/hotel-offers",
      {
        hotelIds: id,
        adults,
        roomQuantity: 1,
        checkInDate: startDate,
        checkOutDate: endDate
      },
      { ttlMs: 2 * 60 * 1000 }
    );
    if (offers?.data?.length) break;
  }

  const first = offers?.data?.[0];
  const offer = first?.offers?.[0];
  const total = Number.parseFloat(offer?.price?.total);

  if (!offer || Number.isNaN(total)) {
    return { stay: null, candidates };
  }

  return {
    stay: {
      name: first?.hotel?.name ?? "Hotel stay",
      rating: first?.hotel?.rating ?? null,
      address: [first?.hotel?.address?.lines?.join(", "), first?.hotel?.address?.cityName]
        .filter(Boolean)
        .join(", "),
      nights: diffNights(startDate, endDate),
      total,
      currency: offer?.price?.currency
    },
    candidates
  };
};

app.post("/api/search", async (req, res) => {
  const payload = req.body ?? {};
  const { errors, value } = validateSearchRequest(payload);
  if (errors) {
    res.status(400).json({ error: "Invalid request", details: errors, requestId: req.id });
    return;
  }

  const request = {
    ...fallbackRequest,
    ...value,
    origin: (value.origin || fallbackRequest.origin).toUpperCase(),
    destination: value.destination || fallbackRequest.destination,
    travelers: Number(value.travelers || fallbackRequest.travelers),
    budget: Number(value.budget || fallbackRequest.budget)
  };

  const liveResults = {
    notes: [],
    debug: {
      providerStatus: [],
      amadeus: {
        env: process.env.AMADEUS_ENV || "test",
        baseUrl: AMADEUS_BASE_URL
      }
    }
  };

  try {
    const flight = await searchFlights({
      origin: request.origin,
      destination: request.destination,
      startDate: request.startDate,
      adults: request.travelers
    });

    if (flight) {
      liveResults.flight = flight;
      liveResults.currency = flight.currency ?? liveResults.currency;
      liveResults.debug.providerStatus.push({
        type: "flights",
        provider: "Amadeus",
        status: "live"
      });
    } else {
      liveResults.debug.providerStatus.push({
        type: "flights",
        provider: "Amadeus",
        status: "mock"
      });
    }
  } catch (error) {
    liveResults.notes.push("Live flight search unavailable; showing estimates.");
    liveResults.debug.providerStatus.push({
      type: "flights",
      provider: "Amadeus",
      status: "mock"
    });
  }

  try {
    const stay = await searchHotels({
      destination: request.destination,
      startDate: request.startDate,
      endDate: request.endDate,
      adults: request.travelers,
      hotelId: request.hotelId
    });

    liveResults.stayCandidates = stay?.candidates ?? [];

    if (stay?.stay) {
      liveResults.stay = stay.stay;
      liveResults.currency = stay.stay.currency ?? liveResults.currency;
      liveResults.debug.providerStatus.push({
        type: "stays",
        provider: "Amadeus",
        status: "live"
      });
    } else {
      liveResults.notes.push("No rooms available for the selected dates. Pick a hotel to continue.");
      liveResults.debug.providerStatus.push({
        type: "stays",
        provider: "Amadeus",
        status: "mock"
      });
    }
  } catch (error) {
    liveResults.notes.push("Live hotel search unavailable; showing estimates.");
    liveResults.debug.providerStatus.push({
      type: "stays",
      provider: "Amadeus",
      status: "mock"
    });
  }

  if (!AMADEUS_CLIENT_ID || !AMADEUS_CLIENT_SECRET) {
    liveResults.notes.push("Missing Amadeus credentials; returning mock providers.");
    liveResults.debug.amadeus.auth = "missing";
  } else if (amadeusTokenError) {
    liveResults.notes.push("Amadeus auth failed; returning mock providers.");
    liveResults.debug.amadeus.auth = "failed";
    liveResults.debug.amadeus.error = amadeusTokenError;
  } else if (amadeusLastError) {
    liveResults.notes.push("Amadeus request failed; returning mock providers.");
    liveResults.debug.amadeus.error = amadeusLastError;
  }

  res.json(buildQuote(request, liveResults));
});

app.get("/api/locations", async (req, res) => {
  const keyword = String(req.query.keyword || "").trim();

  if (!keyword || keyword.length < 2) {
    res.json({ data: [] });
    return;
  }

  const data = await amadeusGet(
    "/v1/reference-data/locations",
    {
      keyword,
      subType: "AIRPORT,CITY"
    },
    { ttlMs: 6 * 60 * 60 * 1000 }
  );

  if (!data) {
    res.status(503).json({
      error: "Unable to fetch locations",
      details: amadeusTokenError || amadeusLastError || null,
      requestId: req.id
    });
    return;
  }

  const lowered = keyword.toLowerCase();
  const locations = (data.data || [])
    .map((item) => {
      const name = item.name || "";
      const cityName = item.address?.cityName || "";
      const haystack = `${name} ${cityName} ${item.iataCode || ""}`.toLowerCase();
      const starts = name.toLowerCase().startsWith(lowered) || cityName.toLowerCase().startsWith(lowered);
      const includes = haystack.includes(lowered);
      return {
        score: starts ? 2 : includes ? 1 : 0,
        payload: item
      };
    })
    .sort((a, b) => b.score - a.score)
    .slice(0, 15)
    .map(({ payload: item }) => ({
      name: item.name,
      cityName: item.address?.cityName,
      countryCode: item.address?.countryCode,
      iataCode: item.iataCode,
      type: item.subType
    }));

  res.json({ data: locations });
});

if (isProd) {
  const distPath = path.resolve(__dirname, "../dist");
  app.use(express.static(distPath));
  app.get("*", (_req, res) => {
    res.sendFile(path.join(distPath, "index.html"));
  });
}

app.use((err, req, res, _next) => {
  if (err?.message?.includes("CORS")) {
    res.status(403).json({ error: "CORS blocked", requestId: req.id });
    return;
  }
  res.status(500).json({ error: "Server error", requestId: req.id });
});

const server = app.listen(PORT, () => {
  logger.info({ port: PORT }, "Tripwise API listening");
});

const shutdown = (signal) => {
  logger.info({ signal }, "Shutting down server");
  server.close(() => process.exit(0));
  setTimeout(() => process.exit(1), 10_000).unref();
};

process.on("SIGINT", () => shutdown("SIGINT"));
process.on("SIGTERM", () => shutdown("SIGTERM"));
