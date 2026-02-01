# Tripwise API (Mocked)

This API is a lightweight Express service that returns mocked flight, hotel, transport, and activity data
so the Tripwise UI can demonstrate a full booking flow. Swap the mocked providers for real integrations
when you have API keys available.

## Quick start

```bash
npm install
npm run dev:server
```

## Environment

Copy `.env.example` to `.env` and update values as needed:

```bash
cp server/.env.example server/.env
```

## Endpoints

| Method | Path         | Description                        |
| ------ | ------------ | ---------------------------------- |
| GET    | `/api/health` | Health check                       |
| POST   | `/api/search` | Returns a mocked Tripwise itinerary |

The `/api/search` payload accepts fields like `origin`, `destination`, `startDate`, `endDate`, `travelers`,
`budget`, and `interests`.
