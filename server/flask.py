
from flask import Flask, request
import random
import base64

app = Flask(__name__)

def rand_qkey(num=None):
    qk_dic = {1:'vuoEaV0hka6heNux56zstFKboQgvGDxm', 2:'ARk7Cuqs3ZyBQmJX91uZ4OBBrNPuWTDw',3:'lEFMecGPXuG3v8YslItBcEQHBdV8oWxF'}
    #print(len(qk_dic))
    r = random.randint(1,len(qk_dic))
    if num != None:
        r = int(num)
    
    return qk_dic.get(r)

def unhash_qkey(hqkey, method):
    print(hqkey)
    hqk_dic = dict()
    if method == 'BIKE-L5':
        hqk_dic = {
        b'vuoEaV0h\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'vuoEaV0hka6heNux56zstFKboQgvGDxm', 
        b'ARk7Cuqs\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'ARk7Cuqs3ZyBQmJX91uZ4OBBrNPuWTDw', 
        b'lEFMecGP\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'lEFMecGPXuG3v8YslItBcEQHBdV8oWxF'}
    elif method == 'Classic-McEliece-6688128':
        hqk_dic = {
        b'ARhS+\x81uE\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'vuoEaV0hka6heNux56zstFKboQgvGDxm', 
        }
    elif method == 'Classic-McEliece-6688128f':
        hqk_dic = {
        b'ARhS+\x81uE\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'vuoEaV0hka6heNux56zstFKboQgvGDxm', 
        }
    elif method == 'Classic-McEliece-6960119':
        hqk_dic = {
        b'ARhS+\x81uE\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'vuoEaV0hka6heNux56zstFKboQgvGDxm', 
        }
    elif method == 'Classic-McEliece-6960119f':
        hqk_dic = {
        b'ARhS+\x81uE\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'vuoEaV0hka6heNux56zstFKboQgvGDxm', 
        }
    elif method == 'Classic-McEliece-8192128':
        hqk_dic = {
        b'ARhS+\x81uE\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'vuoEaV0hka6heNux56zstFKboQgvGDxm', 
        }
    elif method == 'Classic-McEliece-8192128f':
        hqk_dic = {
        b'ARhS+\x81uE\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'vuoEaV0hka6heNux56zstFKboQgvGDxm', 
        }
    elif method == 'HQC-256':
        hqk_dic = {
        b'vuoEaV0hka6heNux56zstFKboQgvGDxm':'vuoEaV0hka6heNux56zstFKboQgvGDxm', 
        b'ARk7Cuqs3ZyBQmJX91uZ4OBBrNPuWTDw':'ARk7Cuqs3ZyBQmJX91uZ4OBBrNPuWTDw', 
        b'lEFMecGPXuG3v8YslItBcEQHBdV8oWxF':'lEFMecGPXuG3v8YslItBcEQHBdV8oWxF'}
    elif method == 'Kyber1024':
        hqk_dic = {
        b'\xe3\x1cu\xd9U\xfb\x1b]\xe2\x9d\xc3\x89\x15\xed\x7f\xc1\xd17N\xd1\x87\x9a\xac\x06\xd81O\x87x=\xe1"':'vuoEaV0hka6heNux56zstFKboQgvGDxm', 
        b'\xc2\x98]\xc3:|v\xc6\xd4\xdb\xa9\x11\xe1\x1a\xaf\x95\xe5\xed\xc0\xc2\x8f8\xe7[\xb9\xcdb"\xc3\xd9\xfb<':'ARk7Cuqs3ZyBQmJX91uZ4OBBrNPuWTDw', 
        b'\x88\xe8\xc1\xba\x9d;Zz\xf4"\x05\x94Q\xe7P\xb9\xc3\x07\xa2\xb0\x96P:\x96c~c\xd3\xec\xc8\x83\x94':'lEFMecGPXuG3v8YslItBcEQHBdV8oWxF'}
    elif method == 'FrodoKEM-1344-AES':
        hqk_dic = {
        b'vuoEaV0h\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'vuoEaV0hka6heNux56zstFKboQgvGDxm', 
        b'ARk7Cuqs\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'ARk7Cuqs3ZyBQmJX91uZ4OBBrNPuWTDw', 
        b'lEFMecGP\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'lEFMecGPXuG3v8YslItBcEQHBdV8oWxF'}
    elif method == 'FrodoKEM-1344-SHAKE':
        hqk_dic = {
        b'vuoEaV0h\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'vuoEaV0hka6heNux56zstFKboQgvGDxm', 
        b'ARk7Cuqs\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'ARk7Cuqs3ZyBQmJX91uZ4OBBrNPuWTDw', 
        b'lEFMecGP\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00':'lEFMecGPXuG3v8YslItBcEQHBdV8oWxF'}
    
    res = hqk_dic.get(hqkey)
    print(res)
    return res



@app.route('/BIKE-L5')
def bike():
    b = request.args.get('v')
    if b:
        tmp = b.encode("utf-8")
        decode_b = base64.urlsafe_b64decode(tmp)
        res = unhash_qkey(decode_b,'BIKE-L5')
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'
    else:
        res = rand_qkey()
        print(res)
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'

@app.route('/Classic-McEliece-6688128')
def c6():
    c6 = request.args.get('v')
    if c6:
        tmp = c6.encode("utf-8")
        decode_c6 = base64.urlsafe_b64decode(tmp)
        res = unhash_qkey(decode_c6,'Classic-McEliece-6688128')
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'
    else:
        print("Classic-McEliece")
        res = rand_qkey(1)
        print(res)
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'


@app.route('/Classic-McEliece-6688128f')
def c6f():
    c6f = request.args.get('v')
    if c6f:
        tmp = c6f.encode("utf-8")
        decode_c6f = base64.urlsafe_b64decode(tmp)
        res = unhash_qkey(decode_c6f,'Classic-McEliece-6688128f')
        if res == None:
            return 'Error occured\n'
        else:
                return f'{res}'
    else:
        print("Classic-McEliece")
        res = rand_qkey(1)
        print(res)
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'

@app.route('/Classic-McEliece-6960119')
def c9():
    c9 = request.args.get('v')
    if c9:
        tmp = c9.encode("utf-8")
        decode_c9 = base64.urlsafe_b64decode(tmp)
        res = unhash_qkey(decode_c9,'Classic-McEliece-6960119')
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'
    else:
        print("Classic-McEliece")
        res = rand_qkey(1)
        print(res)
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'

@app.route('/Classic-McEliece-6960119f')
def c9f():
    c9f = request.args.get('v')
    if c9f:
        tmp = c9f.encode("utf-8")
        decode_c9f = base64.urlsafe_b64decode(tmp)
        res = unhash_qkey(decode_c9f,'Classic-McEliece-6960119f')
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'
    else:
        print("Classic-McEliece")
        res = rand_qkey(1)
        print(res)
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'

@app.route('/Classic-McEliece-8192128')
def c8():
    c8 = request.args.get('v')
    if c8:
        tmp = c8.encode("utf-8")
        decode_c8 = base64.urlsafe_b64decode(tmp)
        res = unhash_qkey(decode_c8,'Classic-McEliece-8192128')
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'
    else:
        print("Classic-McEliece")
        res = rand_qkey(1)
        print(res)
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'


@app.route('/Classic-McEliece-8192128f')
def c8f():
    c8f = request.args.get('v')
    if c8f:
        tmp = c8f.encode("utf-8")
        decode_c8f = base64.urlsafe_b64decode(tmp)
        res = unhash_qkey(decode_c8f,'Classic-McEliece-8192128f')
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'
    else:
        print("Classic-McEliece")
        res = rand_qkey(1)
        print(res)
        if res == None:
            return 'Error occured\n'

@app.route('/HQC-256')
def h():
    h = request.args.get('v')
    if h:
        print('HQC-256')
        tmp = h.encode("utf-8")
        decode_h = base64.urlsafe_b64decode(tmp)
        res = unhash_qkey(decode_h,'HQC-256')
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'
    else:
        print('HQC-256')
        res = rand_qkey()
        print(res)
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'

@app.route('/Kyber1024')
def k():
    k = request.args.get('v')
    if k:
        print('Kyber1024')
        tmp = k.encode("utf-8")
        decode_k = base64.urlsafe_b64decode(tmp)
        res = unhash_qkey(decode_k,'Kyber1024')
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'
    else:
        res = rand_qkey()
        print(res)
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'


@app.route('/FrodoKEM-1344-AES')
def fa():
    fa = request.args.get('v')
    if fa:
        print('FrodoKEM-1344-AES')
        tmp = fa.encode("utf-8")
        decode_fa = base64.urlsafe_b64decode(tmp)
        res = unhash_qkey(decode_fa,'FrodoKEM-1344-AES')
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'
    else:
        res = rand_qkey()
        print(res)
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'

@app.route('/FrodoKEM-1344-SHAKE')
def fs():
    fs = request.args.get('v')
    if fs:
        tmp = fs.encode("utf-8")
        decode_fs = base64.urlsafe_b64decode(tmp)
        res = unhash_qkey(decode_fs,'FrodoKEM-1344-SHAKE')
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'
    else:
        res = rand_qkey()
        print(res)
        if res == None:
            return 'Error occured\n'
        else:
            return f'{res}'



if __name__ == '__main__':
    app.run(port=5000)
