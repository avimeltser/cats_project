import cors from "cors";
import dotenv from "dotenv";
import express from "express";

dotenv.config();

const app = express();
const PORT = Number(process.env.PORT) || 3001;
const CLIENT_ORIGIN = process.env.CORS_ORIGIN || "http://localhost:4173";

app.use(cors({ origin: CLIENT_ORIGIN }));
app.use(express.json());

const providerCatalog = [
  { type: "flights", name: "SkyRoutes", status: "mock" },
  { type: "stays", name: "Stayline", status: "mock" },
  { type: "transport", name: "CityHop", status: "mock" },
  { type: "activities", name: "LocalMuse", status: "mock" }
];

const fallbackRequest = {
  origin: "NYC",
  destination: "Lisbon",
  startDate: "2024-06-18",
  endDate: "2024-06-29",
  travelers: 2,
  budget: 3200,
  interests: "food,culture,coast"
};

const buildQuote = (request) => {
  const flightTotal = 1340;
  const stayTotal = 920;
  const transportTotal = 220;
  const activityTotal = 310;
  const fee = 89;

  return {
    requestId: `tw_${Date.now()}`,
    currency: "USD",
    request,
    providers: providerCatalog,
    itinerary: {
      flights: [
        {
          segment: `${request.origin} → ${request.destination}`,
          cabin: "Economy Flex",
          carrier: "SkyRoutes",
          total: flightTotal
        }
      ],
      stays: [
        {
          name: "Riverside Boutique Hotel",
          nights: 6,
          rating: 4.6,
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
      "Mocked provider data only. Configure live API keys in .env to replace providers.",
      "All totals are estimates and exclude taxes."
    ]
  };
};

app.get("/api/health", (_req, res) => {
  res.json({ status: "ok", timestamp: new Date().toISOString() });
});

app.post("/api/search", (req, res) => {
  const payload = req.body ?? {};
  const request = {
    ...fallbackRequest,
    ...payload,
    origin: (payload.origin || fallbackRequest.origin).toUpperCase(),
    destination: payload.destination || fallbackRequest.destination,
    travelers: Number(payload.travelers || fallbackRequest.travelers),
    budget: Number(payload.budget || fallbackRequest.budget)
  };

  res.json(buildQuote(request));
});

app.listen(PORT, () => {
  console.log(`Tripwise API listening on http://localhost:${PORT}`);
});
