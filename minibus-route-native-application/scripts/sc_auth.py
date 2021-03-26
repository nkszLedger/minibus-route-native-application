
import sys
import json

print('Number of arguments:', len(sys.argv), 'arguments.')
print('Argument List 1:', str(sys.argv[1]))

''' arg1 - url'''
''' arg2 - email'''
''' arg3 - password'''

import http.client

conn = http.client.HTTPSConnection( str(sys.argv[1]) )
payload = ''
headers = {'Content-type': 'application/json'}
conn.request("POST", "/api/login?email=" + str(sys.argv[2]) + "&" + "password=" + str(sys.argv[3]), payload, headers)
res = conn.getresponse()
data = json.loads( res.read().decode('utf-8') )
f = open("scripts/out.txt", "w")
f.write( data['success']['token'] )
f.close()
quit() 
