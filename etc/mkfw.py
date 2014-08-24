#!/usr/bin/env python

#
# Firmware bundle generator
#

import argparse
import json
import base64
import subprocess

#
# Firmware description structure
#
desc = {
    'magic':            'NodesV1',
}

#
# Images to be bundled
#
images = {
    'master':   0,
    'power_v1': 1,
    'power_v3': 2
}

# Parse commandline
parser = argparse.ArgumentParser(description='LIN Nodes firmware bundle generator')
parser.add_argument('--dir',            action='store', help='directory containing node build directories')
parser.add_argument('--version',        action='store', help='set a version string')
parser.add_argument('--description',    action='store', help='set a longer description')
parser.add_argument('--git_identity',   action='store', help='the working directory to check for git identity')
args = parser.parse_args()

if args.version != None:
    desc['version']     = str(args.version)
if args.description != None:
    desc['description'] = str(args.description)
if args.git_identity != None:
    cmd = ' '.join(['git', '--git-dir', args.git_identity + '/.git', 'describe', '--always', '--dirty'])
    p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE).stdout
    desc['git_identity']    = str(p.read().strip())
    p.close()

for image in images:
    f = open(args.dir + '/' + image + '/' + image + '.bin', 'rb')
    bytes = f.read()
    f.close()
    desc['image_' + image] = base64.b64encode(bytes).decode('utf-8')
    desc['id_' + image] = images[image]

print(json.dumps(desc, indent=4))
