import { useState } from "react";

const highlights = [
  {
    title: "One checkout, one itinerary",
    description:
      "Book flights, stays, ground transport, and attractions in a single flow with unified support."
  },
  {
    title: "Concierge-level planning",
    description:
      "Smart bundling surfaces the best routes, hotel blocks, and timed tickets without tab chaos."
  },
  {
    title: "Transparent pricing",
    description:
      "See itemized costs, fees, and live savings before you confirm so there are no surprises."
  }
];

const steps = [
  {
    label: "Step 1",
    title: "Tell us the vibe",
    description:
      "Choose dates, budget, and the pace you want. Tripwise curates the right flights and stays."
  },
  {
    label: "Step 2",
    title: "Lock the essentials",
    description:
      "Approve the itinerary, then add transportation, tours, and tickets in one consolidated cart."
  },
  {
    label: "Step 3",
    title: "Travel with confidence",
    description:
      "Get a unified itinerary, live alerts, and a single support team when plans change."
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

export default function App() {
  const [formData, setFormData] = useState({
    origin: "NYC",
    destination: "Lisbon",
    startDate: "2024-06-18",
    endDate: "2024-06-29",
    travelers: 2,
    budget: 3200,
    interests: "food,culture,coast"
  });
  const [quote, setQuote] = useState(null);
  const [isLoading, setIsLoading] = useState(false);
  const [error, setError] = useState("");

  const handleChange = (event) => {
    const { name, value } = event.target;
    setFormData((current) => ({ ...current, [name]: value }));
  };

  const handleSubmit = async (event) => {
    event.preventDefault();
    setIsLoading(true);
    setError("");

    try {
      const response = await fetch("/api/search", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(formData)
      });

      if (!response.ok) {
        throw new Error("Unable to fetch itinerary. Please try again.");
      }

      const data = await response.json();
      setQuote(data);
    } catch (err) {
      setError(err.message);
      setQuote(null);
    } finally {
      setIsLoading(false);
    }
  };

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
          <button className="secondary-button">Join waitlist</button>
        </nav>

        <div className="hero-content">
          <div>
            <p className="eyebrow">Plan every step, pay once.</p>
            <h1>One checkout for your entire trip.</h1>
            <p className="hero-body">
              Tripwise bundles flights, hotels, transportation, and experiences into a single itinerary
              so you can stop juggling tabs and start traveling.
            </p>
            <div className="hero-actions">
              <button className="primary-button">Build my trip</button>
              <button className="ghost-button">See a sample itinerary</button>
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
      </header>

      <section className="section" id="live">
        <div className="section-header">
          <h2>Live trip orchestration demo.</h2>
          <p>
            Submit a quick request and the Tripwise API returns a complete itinerary, pricing, and provider
            mix. Replace mocked providers with live integrations when you are ready.
          </p>
        </div>
        <div className="live-demo">
          <form className="planner-form" onSubmit={handleSubmit}>
            <div className="input-grid">
              <label>
                Origin
                <input name="origin" value={formData.origin} onChange={handleChange} />
              </label>
              <label>
                Destination
                <input name="destination" value={formData.destination} onChange={handleChange} />
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
            <button className="primary-button" type="submit" disabled={isLoading}>
              {isLoading ? "Building itinerary..." : "Build live itinerary"}
            </button>
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
                <div className="quote-grid">
                  <div className="quote-card">
                    <h4>Flights</h4>
                    <p>{quote.itinerary.flights[0].segment}</p>
                    <strong>${quote.itinerary.flights[0].total}</strong>
                  </div>
                  <div className="quote-card">
                    <h4>Stays</h4>
                    <p>{quote.itinerary.stays[0].name}</p>
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
                <ul className="quote-notes">
                  {quote.notes.map((note) => (
                    <li key={note}>{note}</li>
                  ))}
                </ul>
              </>
            ) : (
              <div className="quote-empty">
                <h3>No itinerary yet</h3>
                <p>Submit the form to pull a live itinerary from the Tripwise API.</p>
              </div>
            )}
          </div>
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
        <button className="primary-button">Get early access</button>
      </footer>
    </div>
  );
}
