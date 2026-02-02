import { useEffect, useState } from "react";

const highlights = [
  {
    title: "One checkout, one itinerary",
    description:
      "Bundle flights, stays, transfers, and tickets in one place with a single confirmation."
  },
  {
    title: "Concierge-level planning",
    description: "Get a curated plan in minutes, then tweak it with live availability."
  },
  {
    title: "Transparent pricing",
    description: "See line-item costs and fees before you book."
  }
];

const steps = [
  {
    label: "Step 1",
    title: "Tell us the vibe",
    description: "Pick dates, budget, and pace. We pull live options."
  },
  {
    label: "Step 2",
    title: "Lock the essentials",
    description: "Approve the plan and add extras in one checkout."
  },
  {
    label: "Step 3",
    title: "Travel with confidence",
    description: "One itinerary, live updates, and a single support line."
  }
];

const benefits = [
  {
    title: "Live availability, zero tab chaos",
    description: "Search once and keep everything synced in a single plan."
  },
  {
    title: "Designed for teams and families",
    description: "Share shortlists, collect preferences, approve together."
  },
  {
    title: "Smart budget guardrails",
    description: "See trade-offs instantly when prices move."
  }
];

const coverage = [
  "Global flights",
  "Hotels + stays",
  "Airport transfers",
  "Rail & rides",
  "Attractions",
  "Dining",
  "Travel insurance",
  "24/7 support"
];

const trustMarks = ["Amadeus", "Stripe", "Google Places", "Uber", "National Rail"];

const testimonials = [
  {
    name: "Ava Klein",
    role: "Operations Lead, Lumina",
    quote:
      "Planning time dropped fast. The shared itinerary killed the email chaos."
  },
  {
    name: "Mason Ortega",
    role: "Founder, IndieAtlas",
    quote:
      "Flights, hotel, transfers, tickets — one flow, one confirmation."
  },
  {
    name: "Nora Patel",
    role: "Family travel coordinator",
    quote:
      "Budget alerts + live updates made it feel effortless."
  }
];

const faqs = [
  {
    question: "Do you book everything for me?",
    answer:
      "You approve the plan first. Then we book in a single checkout."
  },
  {
    question: "Is this for business trips or leisure?",
    answer:
      "Both. Teams use it for group travel, families for multi-city trips."
  },
  {
    question: "Can I change plans after booking?",
    answer:
      "Yes. One itinerary and one support channel for changes."
  }
];

export default function App() {
  const defaultFormData = {
    origin: "NYC",
    destination: "Lisbon",
    startDate: "2026-03-10",
    endDate: "2026-03-20",
    travelers: 2,
    budget: 3200,
    interests: "food,culture,coast",
    bookingMode: "autopilot",
    hotelId: ""
  };
  const [formData, setFormData] = useState(defaultFormData);
  const [quote, setQuote] = useState(null);
  const [isLoading, setIsLoading] = useState(false);
  const [error, setError] = useState("");
  const [originSuggestions, setOriginSuggestions] = useState([]);
  const [destinationSuggestions, setDestinationSuggestions] = useState([]);
  const [activeField, setActiveField] = useState(null);
  const [theme, setTheme] = useState("light");

  const trackEvent = (name, payload = {}) => {
    try {
      if (window.dataLayer) {
        window.dataLayer.push({ event: name, ...payload });
      }
      window.dispatchEvent(new CustomEvent("tw:track", { detail: { name, payload } }));
    } catch {
      // Ignore analytics errors in client.
    }
  };

  const scrollToLive = () => {
    document.querySelector("#live")?.scrollIntoView({ behavior: "smooth" });
  };

  const toggleTheme = () => {
    setTheme((current) => (current === "dark" ? "light" : "dark"));
  };

  const handleChange = (event) => {
    const { name, value } = event.target;
    setFormData((current) => ({ ...current, [name]: value }));
  };

  const handleSelect = (field, item) => {
    const label = item.custom
      ? item.value
      : item.iataCode
        ? `${item.iataCode}`
        : item.cityName || item.name;
    setFormData((current) => ({ ...current, [field]: label }));
    setActiveField(null);
    if (field === "origin") setOriginSuggestions([]);
    if (field === "destination") setDestinationSuggestions([]);
    trackEvent("location_select", { field, label });
  };

  const submitSearch = async (payload, event) => {
    if (event) event.preventDefault();
    setIsLoading(true);
    setError("");
    setActiveField(null);

    if (!formData.origin || !formData.destination || !formData.startDate || !formData.endDate) {
      setError("Please complete origin, destination, and dates.");
      setIsLoading(false);
      return;
    }

    if (new Date(formData.endDate) <= new Date(formData.startDate)) {
      setError("End date must be after start date.");
      setIsLoading(false);
      return;
    }

    try {
      const response = await fetch("/api/search", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(payload)
      });

      if (!response.ok) {
        const data = await response.json().catch(() => null);
        const detail = data?.details?.join(" ") || data?.error || null;
        throw new Error(detail || "Unable to fetch itinerary. Please try again.");
      }

      const data = await response.json();
      setQuote(data);
      trackEvent("search_success", { origin: payload.origin, destination: payload.destination });
    } catch (err) {
      setError(err.message);
      setQuote(null);
      trackEvent("search_error", { message: err.message });
    } finally {
      setIsLoading(false);
    }
  };

  const handleSubmit = (event) => {
    submitSearch(formData, event);
  };

  const handleHotelSelect = (hotel) => {
    const updated = { ...formData, hotelId: hotel.hotelId };
    setFormData(updated);
    submitSearch(updated);
  };

  const fetchLocations = async (value, setter) => {
    const query = value.trim();
    if (query.length < 2) {
      setter([]);
      return;
    }

    try {
      const response = await fetch(`/api/locations?keyword=${encodeURIComponent(query)}`);
      if (!response.ok) {
        setter([]);
        return;
      }
      const data = await response.json();
      setter(data.data || []);
    } catch {
      setter([]);
    }
  };

  useEffect(() => {
    const value = formData.origin;
    const timeout = setTimeout(() => {
      fetchLocations(value, setOriginSuggestions);
    }, 250);
    return () => clearTimeout(timeout);
  }, [formData.origin]);

  useEffect(() => {
    const value = formData.destination;
    const timeout = setTimeout(() => {
      fetchLocations(value, setDestinationSuggestions);
    }, 250);
    return () => clearTimeout(timeout);
  }, [formData.destination]);

  const buildSuggestions = (list, value) => {
    const trimmed = value.trim();
    if (!trimmed) return list;
    const hasExact = list.some(
      (item) =>
        item.iataCode?.toLowerCase() === trimmed.toLowerCase() ||
        item.name?.toLowerCase() === trimmed.toLowerCase() ||
        item.cityName?.toLowerCase() === trimmed.toLowerCase()
    );
    if (hasExact) return list;
    return [
      ...list,
      {
        custom: true,
        name: `Use "${trimmed}"`,
        value: trimmed
      }
    ];
  };

  useEffect(() => {
    try {
      const saved = localStorage.getItem("tripwise.form");
      if (saved) {
        const parsed = JSON.parse(saved);
        setFormData((current) => ({ ...current, ...parsed }));
      }
    } catch {
      // Ignore invalid stored data.
    }
  }, []);

  useEffect(() => {
    try {
      localStorage.setItem("tripwise.form", JSON.stringify(formData));
    } catch {
      // Ignore storage errors.
    }
  }, [formData]);

  useEffect(() => {
    try {
      const saved = localStorage.getItem("tripwise.theme");
      if (saved === "dark" || saved === "light") {
        setTheme(saved);
      }
    } catch {
      // Ignore storage errors.
    }
  }, []);

  useEffect(() => {
    document.body.dataset.theme = theme;
    try {
      localStorage.setItem("tripwise.theme", theme);
    } catch {
      // Ignore storage errors.
    }
  }, [theme]);

  return (
    <div className="app">
      <header className="hero">
        <nav className="nav">
          <div className="logo">Tripwise</div>
          <div className="nav-links">
            <a href="#how">How it works</a>
            <a href="#coverage">Coverage</a>
            <a href="#pricing">Pricing</a>
          </div>
          <div className="nav-actions">
            <button className="icon-button" onClick={toggleTheme} aria-label="Toggle theme">
              {theme === "dark" ? (
                <svg viewBox="0 0 24 24" aria-hidden="true">
                  <path d="M12 3.5a1 1 0 0 1 1 1V6a1 1 0 1 1-2 0V4.5a1 1 0 0 1 1-1zm6.36 2.64a1 1 0 0 1 0 1.41l-1.06 1.06a1 1 0 1 1-1.41-1.41l1.06-1.06a1 1 0 0 1 1.41 0zM12 8a4 4 0 1 0 0 8 4 4 0 0 0 0-8zm8.5 3a1 1 0 0 1 0 2H19a1 1 0 1 1 0-2h1.5zM7.11 7.11a1 1 0 0 1-1.41 0L4.64 6.05a1 1 0 0 1 1.41-1.41l1.06 1.06a1 1 0 0 1 0 1.41zM12 18a1 1 0 0 1 1 1v1.5a1 1 0 1 1-2 0V19a1 1 0 0 1 1-1zm7.05.95a1 1 0 0 1-1.41 1.41l-1.06-1.06a1 1 0 0 1 1.41-1.41l1.06 1.06zM3.5 12a1 1 0 0 1 1-1H6a1 1 0 1 1 0 2H4.5a1 1 0 0 1-1-1zm3.61 5.89a1 1 0 0 1-1.41 0l-1.06-1.06a1 1 0 1 1 1.41-1.41l1.06 1.06a1 1 0 0 1 0 1.41z" />
                </svg>
              ) : (
                <svg viewBox="0 0 24 24" aria-hidden="true">
                  <path d="M20.5 14.2a1 1 0 0 0-1.25-1.25 6.5 6.5 0 0 1-8.2-8.2A1 1 0 0 0 9.8 3.5 8.5 8.5 0 1 0 20.5 14.2z" />
                </svg>
              )}
            </button>
            <button className="secondary-button" onClick={scrollToLive}>
              Join waitlist
            </button>
          </div>
        </nav>

        <div className="hero-content">
          <div>
            <p className="eyebrow">Plan every step, pay once.</p>
            <h1>One checkout for your whole trip.</h1>
            <p className="hero-body">
              Tripwise bundles flights, stays, and experiences into one itinerary so you can stop juggling tabs.
            </p>
            <div className="hero-highlights">
              <span>Live availability</span>
              <span>Shared itineraries</span>
              <span>Unified support</span>
            </div>
            <div className="hero-actions">
              <button className="primary-button" onClick={scrollToLive}>
                Build my trip
              </button>
              <button className="ghost-button" onClick={scrollToLive}>
                View sample
              </button>
            </div>
            <div className="hero-meta">
              <div>
                <strong>48 hrs</strong>
                <span>average planning time saved</span>
              </div>
              <div>
                <strong>120+</strong>
                <span>destinations supported</span>
              </div>
              <div>
                <strong>1 fee</strong>
                <span>for full-service booking</span>
              </div>
            </div>
          </div>

          <div className="hero-card">
            <div className="card-header">
              <div>
                <span className="pill">Early access</span>
                <h3>Trip builder</h3>
              </div>
              <span className="status">Live quote</span>
            </div>
            <div className="card-row">
              <span>Route</span>
              <strong>NYC → Lisbon → Porto</strong>
            </div>
            <div className="card-row">
              <span>Dates</span>
              <strong>Jun 18 - Jun 29</strong>
            </div>
            <div className="card-row">
              <span>Travel style</span>
              <strong>Food + culture + coast</strong>
            </div>
            <div className="card-summary">
              <div>
                <p>Flights + Hotels</p>
                <strong>$2,460</strong>
              </div>
              <div>
                <p>Transfers + Tours</p>
                <strong>$740</strong>
              </div>
              <div>
                <p>Tripwise fee</p>
                <strong>$89</strong>
              </div>
            </div>
            <button className="primary-button full">Reserve trip</button>
          </div>
        </div>
        <div className="trust-bar">
          <p>Powered by</p>
          <div className="trust-logos">
            {trustMarks.map((item) => (
              <span key={item}>{item}</span>
            ))}
          </div>
        </div>
      </header>

      <section className="section" id="live">
        <div className="section-header">
          <h2>Live itinerary builder.</h2>
          <p>
            Send a request and Tripwise returns a full itinerary with live pricing and providers.
          </p>
        </div>
        <div className="live-demo">
          <form className="planner-form" onSubmit={handleSubmit}>
            <div className="mode-wrapper">
              <div className="mode-toggle">
                <label
                  className={`mode-option ${formData.bookingMode === "autopilot" ? "active" : ""}`}
                >
                  <input
                    type="radio"
                    name="bookingMode"
                    value="autopilot"
                    checked={formData.bookingMode === "autopilot"}
                    onChange={handleChange}
                  />
                  <span className="mode-title">
                    <svg viewBox="0 0 24 24" aria-hidden="true">
                      <path d="M4 12a8 8 0 1 1 14.9 3.2l2.1 2.1a1 1 0 0 1-1.4 1.4l-2.2-2.2A8 8 0 0 1 4 12zm8-6a6 6 0 1 0 4.2 10.2 1 1 0 0 1 1.4 0l.3.3A6 6 0 0 0 12 6z" />
                    </svg>
                    Tripwise Autopilot
                  </span>
                  <small>We choose the best options for your budget.</small>
                  <span className="mode-tag">Recommended</span>
                </label>
                <label
                  className={`mode-option ${formData.bookingMode === "copilot" ? "active" : ""}`}
                >
                  <input
                    type="radio"
                    name="bookingMode"
                    value="copilot"
                    checked={formData.bookingMode === "copilot"}
                    onChange={handleChange}
                  />
                  <span className="mode-title">
                    <svg viewBox="0 0 24 24" aria-hidden="true">
                      <path d="M5 6a3 3 0 1 1 5.7 1.3A4 4 0 0 1 12 11a4 4 0 0 1 3-3.7A3 3 0 1 1 19 6v8a5 5 0 0 1-5 5H9a4 4 0 0 1-4-4V6z" />
                    </svg>
                    Tripwise Co‑Pilot
                  </span>
                  <small>You approve each choice before booking.</small>
                </label>
              </div>
              <div className="mode-preview">
                {formData.bookingMode === "autopilot" ? (
                  <>
                    <h4>Autopilot delivers a full plan fast.</h4>
                    <ul>
                      <li>We select the best-value flight + stay combo.</li>
                      <li>Auto-adjust to budget changes.</li>
                      <li>One-click checkout when you’re ready.</li>
                    </ul>
                  </>
                ) : (
                  <>
                    <h4>Co‑Pilot keeps you in control.</h4>
                    <ul>
                      <li>Approve flights, stays, and add-ons step by step.</li>
                      <li>Compare options with live pricing.</li>
                      <li>Lock in only what you want.</li>
                    </ul>
                  </>
                )}
              </div>
            </div>
            <div className="input-grid">
              <label>
                Origin
                <input
                  name="origin"
                  value={formData.origin}
                  onChange={handleChange}
                  onFocus={() => setActiveField("origin")}
                  onBlur={() => setTimeout(() => setActiveField(null), 120)}
                />
                {activeField === "origin" && buildSuggestions(originSuggestions, formData.origin).length ? (
                  <div className="suggestions">
                    {buildSuggestions(originSuggestions, formData.origin).map((item) => (
                      <button
                        type="button"
                        key={`${item.iataCode || "custom"}-${item.name}`}
                        className="suggestion"
                        onMouseDown={() => handleSelect("origin", item)}
                      >
                        <span>{item.name || item.cityName}</span>
                        {item.custom ? (
                          <span>Manual entry</span>
                        ) : (
                          <span>
                            {item.iataCode} · {item.cityName || item.name}
                          </span>
                        )}
                      </button>
                    ))}
                  </div>
                ) : null}
              </label>
              <label>
                Destination
                <input
                  name="destination"
                  value={formData.destination}
                  onChange={handleChange}
                  onFocus={() => setActiveField("destination")}
                  onBlur={() => setTimeout(() => setActiveField(null), 120)}
                />
                {activeField === "destination" &&
                buildSuggestions(destinationSuggestions, formData.destination).length ? (
                  <div className="suggestions">
                    {buildSuggestions(destinationSuggestions, formData.destination).map((item) => (
                      <button
                        type="button"
                        key={`${item.iataCode || "custom"}-${item.name}`}
                        className="suggestion"
                        onMouseDown={() => handleSelect("destination", item)}
                      >
                        <span>{item.name || item.cityName}</span>
                        {item.custom ? (
                          <span>Manual entry</span>
                        ) : (
                          <span>
                            {item.iataCode} · {item.cityName || item.name}
                          </span>
                        )}
                      </button>
                    ))}
                  </div>
                ) : null}
              </label>
              <label>
                Start date
                <input name="startDate" type="date" value={formData.startDate} onChange={handleChange} />
              </label>
              <label>
                End date
                <input name="endDate" type="date" value={formData.endDate} onChange={handleChange} />
              </label>
              <label>
                Travelers
                <input
                  name="travelers"
                  type="number"
                  min="1"
                  value={formData.travelers}
                  onChange={handleChange}
                />
              </label>
              <label>
                Budget (USD)
                <input name="budget" type="number" value={formData.budget} onChange={handleChange} />
              </label>
              <label className="full">
                Interests
                <input name="interests" value={formData.interests} onChange={handleChange} />
              </label>
            </div>
            <p className="form-hint">Tip: Use airport codes for fastest matches (e.g., NYC, TLV).</p>
            <button className="primary-button" type="submit" disabled={isLoading}>
              {isLoading ? "Building itinerary..." : "Build live itinerary"}
            </button>
            {isLoading ? (
              <div className="loading-row" aria-live="polite">
                <span className="spinner" />
                <span>Fetching live availability...</span>
              </div>
            ) : null}
            {error ? <p className="error">{error}</p> : null}
          </form>

          <div className="quote-panel">
            {quote ? (
              <>
                <div className="quote-header">
                  <div>
                    <p className="eyebrow">Request ID</p>
                    <h3>{quote.requestId}</h3>
                  </div>
                  <span className="status">{quote.currency}</span>
                </div>
                <div className="provider-status">
                  {quote.providers.map((provider) => (
                    <span key={provider.type} className={`provider-badge ${provider.status}`}>
                      {provider.name} · {provider.type} · {provider.status}
                    </span>
                  ))}
                </div>
                <div className="quote-grid">
                  <div className="quote-card">
                    <h4>Flights</h4>
                    <p>{quote.itinerary.flights[0].segment}</p>
                    {quote.itinerary.flights[0].carrier ? (
                      <p className="muted">
                        {quote.itinerary.flights[0].carrier}
                        {quote.itinerary.flights[0].carrierCode
                          ? ` (${quote.itinerary.flights[0].carrierCode})`
                          : ""}
                      </p>
                    ) : null}
                    {quote.itinerary.flights[0].departure ? (
                      <p className="muted">
                        Departs: {new Date(quote.itinerary.flights[0].departure).toLocaleString()}
                      </p>
                    ) : null}
                    {quote.itinerary.flights[0].arrival ? (
                      <p className="muted">
                        Arrives: {new Date(quote.itinerary.flights[0].arrival).toLocaleString()}
                      </p>
                    ) : null}
                    {quote.itinerary.flights[0].duration ? (
                      <p className="muted">Duration: {quote.itinerary.flights[0].duration}</p>
                    ) : null}
                    {Number.isFinite(quote.itinerary.flights[0].stops) ? (
                      <p className="muted">Stops: {quote.itinerary.flights[0].stops}</p>
                    ) : null}
                    <strong>${quote.itinerary.flights[0].total}</strong>
                  </div>
                  <div className="quote-card">
                    <h4>Stays</h4>
                    <p>{quote.itinerary.stays[0].name}</p>
                    {quote.itinerary.stays[0].address ? (
                      <p className="muted">{quote.itinerary.stays[0].address}</p>
                    ) : null}
                    {quote.itinerary.stays[0].rating ? (
                      <p className="muted">Rating: {quote.itinerary.stays[0].rating}</p>
                    ) : null}
                    {quote.itinerary.stays[0].nights ? (
                      <p className="muted">Nights: {quote.itinerary.stays[0].nights}</p>
                    ) : null}
                    <strong>${quote.itinerary.stays[0].total}</strong>
                  </div>
                  <div className="quote-card">
                    <h4>Transport</h4>
                    <p>{quote.itinerary.transport[0].mode}</p>
                    <strong>${quote.itinerary.transport[0].total}</strong>
                  </div>
                  <div className="quote-card">
                    <h4>Activities</h4>
                    <p>{quote.itinerary.activities[0].name}</p>
                    <strong>${quote.itinerary.activities[0].total}</strong>
                  </div>
                </div>
                <div className="quote-footer">
                  <div>
                    <p>Total estimate</p>
                    <strong>${quote.totals.total}</strong>
                  </div>
                  <div className="provider-list">
                    {quote.providers.map((provider) => (
                      <span key={provider.type}>
                        {provider.name} · {provider.type}
                      </span>
                    ))}
                  </div>
                </div>
                {formData.bookingMode === "copilot" && quote.stayCandidates?.length ? (
                  <div className="hotel-choices">
                    <h4>Pick a hotel to continue</h4>
                    <div className="hotel-grid">
                      {quote.stayCandidates.map((hotel) => (
                        <button
                          type="button"
                          key={hotel.hotelId}
                          className="hotel-card"
                          onClick={() => handleHotelSelect(hotel)}
                        >
                          <div className="hotel-card-top">
                            <strong>{hotel.name}</strong>
                            <span>{hotel.address || "Address unavailable"}</span>
                          </div>
                          <div className="hotel-rating">
                            <span className="stars">★</span>
                            <span>{hotel.rating ? hotel.rating : "N/A"}</span>
                            <span className="hotel-reviews">
                              {hotel.reviewCount ? `${hotel.reviewCount} reviews` : "—"}
                            </span>
                          </div>
                        </button>
                      ))}
                    </div>
                  </div>
                ) : null}
                {quote.debug?.providerStatus?.length ? (
                  <div className="quote-debug">
                    <h4>Live data status</h4>
                    <div className="quote-debug-grid">
                      {quote.debug.providerStatus.map((item) => (
                        <div key={`${item.type}-${item.provider}`} className="debug-card">
                          <strong>{item.type}</strong>
                          <span>{item.provider}</span>
                          <span className={`debug-status ${item.status}`}>{item.status}</span>
                        </div>
                      ))}
                    </div>
                    {quote.debug?.amadeus?.error ? (
                      <div className="debug-error">
                        <strong>Amadeus error</strong>
                        <span>
                          {quote.debug.amadeus.error.status
                            ? `HTTP ${quote.debug.amadeus.error.status}`
                            : "Request failed"}
                        </span>
                        {quote.debug.amadeus.error.path ? (
                          <span>{quote.debug.amadeus.error.path}</span>
                        ) : null}
                        {quote.debug.amadeus.error.body ? (
                          <pre className="debug-body">{quote.debug.amadeus.error.body}</pre>
                        ) : null}
                      </div>
                    ) : null}
                  </div>
                ) : null}
                <ul className="quote-notes">
                  {quote.notes.map((note) => (
                    <li key={note}>{note}</li>
                  ))}
                </ul>
              </>
            ) : isLoading ? (
              <div className="quote-loading">
                <div className="skeleton heading" />
                <div className="skeleton line" />
                <div className="skeleton line" />
                <div className="skeleton grid" />
              </div>
            ) : (
              <div className="quote-empty">
                <h3>No itinerary yet</h3>
                <p>Submit the form to pull a live itinerary from the Tripwise API.</p>
              </div>
            )}
          </div>
        </div>
      </section>

      <section className="section" id="benefits">
        <div className="section-header">
          <h2>Everything you need to decide fast.</h2>
          <p>Clear trade-offs, shareable shortlists, and one place to manage changes.</p>
        </div>
        <div className="grid three">
          {benefits.map((item) => (
            <article key={item.title} className="card">
              <h3>{item.title}</h3>
              <p>{item.description}</p>
            </article>
          ))}
        </div>
      </section>

      <section className="section" id="how">
        <div className="section-header">
          <h2>All the planning in one place.</h2>
          <p>
            We combine flights, stays, activities, and local transport so you only make decisions that
            matter.
          </p>
        </div>
        <div className="grid three">
          {highlights.map((item) => (
            <article key={item.title} className="card">
              <h3>{item.title}</h3>
              <p>{item.description}</p>
            </article>
          ))}
        </div>
      </section>

      <section className="section alt" id="testimonials">
        <div className="section-header">
          <h2>Teams and travelers trust Tripwise.</h2>
          <p>Concierge-style planning with the speed of a modern booking tool.</p>
        </div>
        <div className="grid three">
          {testimonials.map((item) => (
            <article key={item.name} className="testimonial-card">
              <p>"{item.quote}"</p>
              <div>
                <strong>{item.name}</strong>
                <span>{item.role}</span>
              </div>
            </article>
          ))}
        </div>
      </section>

      <section className="section alt" id="coverage">
        <div className="section-header">
          <h2>Everything you need to travel well.</h2>
          <p>We partner with top providers so every part of your trip is covered.</p>
        </div>
        <div className="coverage">
          {coverage.map((item) => (
            <div key={item} className="coverage-item">
              {item}
            </div>
          ))}
        </div>
      </section>

      <section className="section" id="faq">
        <div className="section-header">
          <h2>Questions, answered.</h2>
          <p>Everything you need to know before you plan.</p>
        </div>
        <div className="faq-grid">
          {faqs.map((item) => (
            <article key={item.question} className="faq-card">
              <h3>{item.question}</h3>
              <p>{item.answer}</p>
            </article>
          ))}
        </div>
      </section>

      <section className="section" id="pricing">
        <div className="section-header">
          <h2>A simple fee that saves hours.</h2>
          <p>Pay once to have a concierge plan, book, and manage every detail.</p>
        </div>
        <div className="grid two">
          <div className="pricing-card">
            <h3>Tripwise Standard</h3>
            <p className="price">$89 trip planning fee</p>
            <ul>
              <li>Curated flight + hotel bundle</li>
              <li>Airport transfers + local transport</li>
              <li>Attraction and dining reservations</li>
              <li>24/7 unified support</li>
            </ul>
            <button className="primary-button full">Start planning</button>
          </div>
          <div className="pricing-card highlight">
            <div className="badge">Most popular</div>
            <h3>Tripwise Plus</h3>
            <p className="price">$149 trip planning fee</p>
            <ul>
              <li>Everything in Standard</li>
              <li>Multi-city optimization</li>
              <li>Premium lounge & insurance add-ons</li>
              <li>Dedicated travel concierge</li>
            </ul>
            <button className="secondary-button full">Talk to concierge</button>
          </div>
        </div>
      </section>

      <section className="section alt" id="steps">
        <div className="section-header">
          <h2>Three steps to takeoff.</h2>
          <p>We do the heavy lifting so you can focus on the fun parts.</p>
        </div>
        <div className="grid three">
          {steps.map((step) => (
            <article key={step.title} className="card step">
              <span className="step-label">{step.label}</span>
              <h3>{step.title}</h3>
              <p>{step.description}</p>
            </article>
          ))}
        </div>
      </section>

      <footer className="footer">
        <div>
          <h3>Ready to plan everything at once?</h3>
          <p>Join the waitlist to get early access and locked-in pricing.</p>
        </div>
        <button className="primary-button" onClick={scrollToLive}>
          Get early access
        </button>
      </footer>
    </div>
  );
}
