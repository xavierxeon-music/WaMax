#!/usr/bin/env python3

import json
import os

os.makedirs('.vscode', exist_ok=True)

launch = {
   'version': '0.2.0',
   'configurations': []
}

standardConfig = {
    'request': 'launch',
    'type': 'chrome',
    'includeLaunchArgs': True,
    'runtimeArgs': [
            '--allow-file-access-from-files'
    ],
    'url': 'http://localhost:8080',
    'webRoot': '${workspaceFolder}'
}


with open('maxgui.json', 'r') as infile:
   content = json.load(infile)

for key, configDict in content.items():

   if (key == '_default'):
      continue

   config = standardConfig.copy()
   config['name'] = configDict['name']
   config['file'] = '${workspaceFolder}/maxgui.html?content=' + key

   launch['configurations'].append(config)

with open('.vscode/launch.json', 'w') as outfile:
   json.dump(launch, outfile, indent=3)
