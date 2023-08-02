from ecdsa import SigningKey, SECP256k1, VerifyingKey

#生成公钥、私钥
def generate_keys():
    sk = SigningKey.generate(curve=SECP256k1)
    vk = sk.get_verifying_key()
    secret_key = sk.to_string().hex()
    verify_key = vk.to_string().hex()
    return secret_key,verify_key

#签名
def ecdsa_sign(secret_key, m):
    sk = SigningKey.from_string(bytes.fromhex(secret_key), curve=SECP256k1)
    signature = sk.sign(bytes(m, 'utf-8'))
    return signature.hex()

#利用函数verify验证签名
def Verify(verify_key, signature, m):
    vk = VerifyingKey.from_string(bytes.fromhex(verify_key), curve=SECP256k1)
    return vk.verify(bytes.fromhex(signature), bytes(m, 'utf-8'))

if __name__ == '__main__':
    sk, pk = generate_keys()
    m = 'hello'
    signature = ecdsa_sign(sk, m)
    print('signature: ', signature)
    print('verification result: ', Verify(pk, signature,m))
