#!/usr/bin/env python3
import ssl
import socket


# conf
addr     = ( "fit.cvut.cz", 443 )
MSG_SIZE = 4096
# create default context
ctx = ssl . SSLContext ( ssl . PROTOCOL_TLSv1_2 )

# these contexts are disabled by default in python 3.5.x
# but we can disable them anyway:
ctx . options &= ssl . OP_NO_SSLv2
ctx . options &= ssl . OP_NO_SSLv3

# set the certification option:
ctx . verify_mode = ssl . CERT_REQUIRED
#ctx . check_hostname = True

ctx . load_verify_locations ( cafile="python.pem", cadata="python.pem" )


# -- used cipher:
# 'ECDHE-RSA-AES256-GCM-SHA384' 
# and we will disable that 
ctx . set_ciphers ( "ALL:!ECDHE-RSA-AES256-GCM-SHA384" )


#print ( ssl . get_default_verify_paths () )
print ( ctx . get_ca_certs() )
# the agreed cipher is now 'ECDHE-RSA-AES

# create socket
sock = socket . socket ( socket . AF_INET, socket . SOCK_STREAM )

# wrap socket
ssock = ctx . wrap_socket ( sock, server_hostname="fit.cvut.cz" )

# connect
ssock . connect ( addr )

# send HTTP GET REQ
ssock . write ( b"GET /student/odkazy HTTP/1.1\r\nConnection: close\r\nHost: fit.cvut.cz\r\n\r\n"  )


_response = ssock . read ( MSG_SIZE )
_format   = ssock . read ( MSG_SIZE )


print ( ssock . cipher () )


f = open ( 'page.html', 'wb' )

while True:
    output = ssock . read ( MSG_SIZE )
    if output == b'':
        break
    f . write ( output )

f . close ()

# get cipher list
print ( "---------------------------------------------------------------------------------------" )
cipherList = ssock . shared_ciphers ()
#for cipher in cipherList:
#    print ( cipher )

ssock . unwrap ()
ssock . close ()


cert = ssl . get_server_certificate ( addr )
with open ( 'certificate.pem', 'w') as f: f . write ( cert )

