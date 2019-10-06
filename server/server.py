#!/usr/bin/env python3

from http.server import BaseHTTPRequestHandler, HTTPServer
import json

CHALLENGES = [{
	"id": 0,
	"prefix": "basilisk:0000000000:",
	"nonce_length": 64,
	"best_nonce": "7cv3PgAwLGnL2albjmfnHGGfy4uNKh0Vv3i49BV58lcBemC2YZOvbaPG0YyX8GVN",
}]

class ChallengesRequestHandler(BaseHTTPRequestHandler):
	def do_GET(self):
		self.send_response(200)
		self.send_header("Content-type", "application/json")
		self.end_headers()
		self.wfile.write(str.encode(json.dumps(CHALLENGES)))

if __name__ == "__main__":
	httpd = HTTPServer(("localhost", 8000), ChallengesRequestHandler)
	httpd.serve_forever()
	print(json.dumps(CHALLENGES))
