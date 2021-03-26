import sys
import json
import http.client

''' arg1 - url '''
''' arg2 - token '''
''' arg3 - subject_type ( military_veterans, employees, members )''' 
''' arg4 - subject_type_id '''

conn = http.client.HTTPSConnection( str(sys.argv[1]) )
payload = ''
token = str( sys.argv[2] )
headers = {
  'Authorization': 'Bearer ' + token
}
address = "/api/" + str( sys.argv[3] ) + "/" + str( sys.argv[4] )

print( address )

conn.request("GET", address, payload, headers)
res = conn.getresponse()
data = json.loads( res.read().decode('utf-8') )
f = open("scripts/out.txt", "w")
f.write( json.dumps(data) )
f.close()
quit() 