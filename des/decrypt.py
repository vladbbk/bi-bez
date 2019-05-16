#!/usr/bin/env python3

import pyDes, sys

DATA_SIZE = 1024

key    = ''
mode   = ''
iv     = ''
chunks = 0
enc    = []
rest   = [] 

# ------------------------------------------------
# get key, mode, 
while True:
    inp = input ( "input 8byte long key: " )
    if len ( inp ) != 8:
        print ( "wrong length, try again!" )
        continue
    key = inp
    break


while True:
    inp = input ( "which mode - ECB or CBC? " )
    if inp != "ECB" and inp != "CBC":
        print ( "wrong mode, try again! " )
        continue
    if inp == "ECB":
        mode = pyDes . ECB
    if inp == "CBC":
        while True:
            inv = input ( "you chose CBC, please provide 8byte long IV: " )
            if len ( inv ) != 8:
                print ( "wrong length, try again! " )
                continue
            iv = inv
            break
        mode = pyDes . CBC
    break

chunks = int ( input ( "how much 8byte chunks you want to decrypt?: " ) )
if chunks == 0:
    exit ( 1 )

# ------------------------------------------------
# setup the DES cipher
des = pyDes . des ( key, mode, iv )
# ------------------------------------------------
pic = open ( "encrypted.bmp", "rb" )

stuff = pic . read ( 10 )

#header = pic . read ( 2 )
#size   = pic . read ( 4 )
#reserv = pic . read ( 4 )
offset = pic . read ( 4 )


imgData = 0

for byte in offset:
    byte = int ( str ( byte ), 16 )
    imgData += byte


print ( "img data starts: ", str ( imgData - 14 ) )

res = pic . read ( imgData )

for chunk in range ( 0, chunks ):
    data = pic . read ( 8 )
    decrypted = des . decrypt ( data) 
    enc . append ( decrypted )
    print ( "encrypted: ", data, end='')
    print ( "  --> ", decrypted )

while True:
    data = pic . read ( DATA_SIZE )
    if data == b'':
        break
    rest . append ( data )

pic . close ()


out = open ( "decrypted.bmp", "wb" )

out . write ( stuff )
out . write ( offset )
out . write ( res )
for chunk in enc:
    out . write ( chunk )

for chunk in rest:
    out . write ( chunk )

out . close ()
