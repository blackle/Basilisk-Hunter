#!/usr/bin/env python3
import sys
import os
import subprocess

TEMPLATE = """
#include <version.h>

const std::string VERSION_STRING("{0}");
const std::string USER_AGENT_STRING("{1}");
"""

def get_git_version(git_exe):
	if not git_exe:
		return ""
	try:
		git_hash = subprocess.check_output([git_exe, "rev-parse", "--short", "HEAD"])
		git_hash = git_hash.decode("utf-8").rstrip()
		try:
			subprocess.check_output([git_exe, "diff", "--quiet"])
		except subprocess.CalledProcessError as diffexec:
			git_hash += "-dirty"
		return git_hash
	except:
		return ""

if __name__ == "__main__":
	script_name, output_path, project_name, version_number, git_exe = sys.argv

	version_string = project_name + " " + version_number
	user_agent = project_name + "/" + version_number

	git_version = get_git_version(git_exe)

	if git_version:
		hash_string = " (" + git_version + ")"
		version_string += hash_string
		user_agent += hash_string

	os.makedirs(os.path.dirname(output_path), exist_ok=True)
	with open(output_path, 'w') as output_file:
		output_file.write(TEMPLATE.format(version_string, user_agent))
