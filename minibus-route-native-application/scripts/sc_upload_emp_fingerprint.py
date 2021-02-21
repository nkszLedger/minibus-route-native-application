import sys

#print('Number of arguments:', len(sys.argv), 'arguments.')
#print('Argument List 0:', str(sys.argv[1]))

''' arg1 - url'''
''' arg2 - token'''
''' arg3 - fingerprint_file_path'''
''' arg4 - employee_id'''

import http.client
import mimetypes
from codecs import encode

#print('Argument List: ', str(sys.argv))

conn = http.client.HTTPSConnection( str(sys.argv[1]) )
data_list = []
boundary = 'wL36Yn8afVp8Ag7AmP8qZ0SA4n1v9T'
data_list.append(encode('--' + boundary))
data_list.append(encode('Content-Disposition: form-data; name=fingerprint; filename={0}'.format(str(sys.argv[3]))))

file_type = mimetypes.guess_type( str(sys.argv[3]) )[0] or 'application/octet-stream'
data_list.append(encode('Content-Type: {}'.format(file_type)))
data_list.append(encode(''))

with open(str(sys.argv[3]), 'rb') as f:
  data_list.append(f.read())
data_list.append(encode('--' + boundary))
data_list.append(encode('Content-Disposition: form-data; name=id;'))

data_list.append(encode('Content-Type: {}'.format('text/plain')))
data_list.append(encode(''))

data_list.append(encode(str(sys.argv[4])))
data_list.append(encode('--'+boundary+'--'))
data_list.append(encode(''))
body = b'\r\n'.join(data_list)
payload = body
token = str(sys.argv[2])
headers = {
  'Authorization': 'Bearer ' + token,
  'Accept': 'application/json',
  'Content-type': 'multipart/form-data; boundary={}'.format(boundary)
}
conn.request("POST", "/api/employeefingerprint/", payload, headers)
res = conn.getresponse()
data = res.read()
f = open("scripts/out.txt", "a"):
    f.write( data.decode("utf-8") )
    f.close()