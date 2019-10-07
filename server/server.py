#!/usr/bin/env python3

from http.server import BaseHTTPRequestHandler, HTTPServer
import json

CHALLENGES = [{
	"id": "0000000000",
	"prefix": "basilisk:0000000000:",
	"nonce_length": 64,
	"best_nonce": "AkD43Rwbud4zUdrfSKWdUnL9zW36CMyBGL8XQwiQnway6wBc1hPqG00cpUtSsq0k",
	"best_hash": "0001e7ef89881a7e3a00a121cf0741905ee9d63091ed5a603521f3ab3b809289",
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
