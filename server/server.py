#!/usr/bin/env python3

# todo: use flask!
# todo: use sqlite!

from http.server import BaseHTTPRequestHandler, HTTPServer
import json
import re

CHALLENGES = [{
	"id": "0000000000",
	"prefix": "basilisk:0000000000:",
	"nonce_length": 64,
	"solution": {
		"hash": "0001e7ef89881a7e3a00a121cf0741905ee9d63091ed5a603521f3ab3b809289",
		"nonce": "AkD43Rwbud4zUdrfSKWdUnL9zW36CMyBGL8XQwiQnway6wBc1hPqG00cpUtSsq0k",
	}
}]

class BasiliskRequestHandler(BaseHTTPRequestHandler):
	def do_GET(self):
		if self.path == "/challenges/" and self.command == "GET":
			self.do_challenge_list()
			return
		match = re.match(r"/challenges/(\w+)", self.path)
		if match and self.command == "POST":
			self.do_set_challenge(match.group(1))
			return;

		self.do_not_found()

	def do_not_found(self):
		self.send_response(404)
		self.send_header("Content-type", "application/json")
		self.end_headers()

	def do_challenge_list(self):
		self.send_response(200)
		self.send_header("Content-type", "application/json")
		self.end_headers()
		self.wfile.write(str.encode(json.dumps(CHALLENGES)))

	def do_set_challenge(self, id):
		self.send_response(200)
		self.send_header("Content-type", "application/json")
		self.end_headers()
		challenge = CHALLENGES[1]
		json.loads(self.rfile.read())
		self.wfile.write(str.encode(json.dumps(challenge)))



if __name__ == "__main__":
	httpd = HTTPServer(("localhost", 8000), BasiliskRequestHandler)
	httpd.serve_forever()
	print(json.dumps(CHALLENGES))
