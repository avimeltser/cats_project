# Tripwise

Unified trip planning demo with live Amadeus integrations (flights + hotels).

## Quick start

1) Create `.env` from the example:

```
cp .env.example .env
```

2) Add your Amadeus credentials to `.env`.

3) Install and run:

```
npm install
npm run dev -- --host
```

UI: `http://localhost:4173`  
API: `http://localhost:3001`

## Environment variables

- `AMADEUS_CLIENT_ID` / `AMADEUS_CLIENT_SECRET`: Required for live data
- `AMADEUS_ENV`: `test` (default) or `production`
- `CORS_ORIGIN`: Comma-separated allowed origins
- `PORT`: API port
- `LOG_LEVEL`: `debug`, `info`, `warn`, `error`
- `NODE_ENV`: `development` or `production`
- `ENABLE_REAL_RATINGS`: Set to `true` only when a real ratings provider is wired

## Production

Build the UI and serve via the API server:

```
npm run build
npm run start
```

The server will host the static build from `dist/`.

## Health check

`GET /api/health`

## Notes

Amadeus test data is limited; use future dates and try multiple destinations if results are empty.
