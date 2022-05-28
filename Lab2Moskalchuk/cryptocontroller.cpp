#include "cryptocontroller.h"
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>



CryptoController::CryptoController (QObject *parent)
{
    //keyHash = passwordToHash("12345").substr(0,32);
}

string CryptoController::passwordToHash(string password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password.c_str(), password.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}

bool CryptoController::encryptString(unsigned char* _key) {
    string text;
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    unsigned char * iv = (unsigned char *)_iv,
            *key = (unsigned char *) (char*) _key;

    if(!EVP_EncryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, key, iv))
        return false;

    unsigned char ciphertext[256];
    unsigned char plaintext[256];

    int sourcetext_len = strlen((char *)strText.substr(0, 256).c_str()),
            len,
            ciphertext_len;

    while (sourcetext_len > 0)
    {
        strcpy((char*)plaintext, (char*)strText.substr(0, 256).c_str());

        if(!EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, sourcetext_len))
            return false;

        strText.erase(0, 256);
        sourcetext_len = strlen((char *)strText.substr(0, 256).c_str());

        string all_data_(ciphertext, ciphertext+len);
        text += all_data_;
    }

    ciphertext_len = len;

    if(!EVP_CipherFinal_ex(ctx, ciphertext + len, &len))
    {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    ciphertext[ciphertext_len] = '\0';

    string _strText(ciphertext, ciphertext+len);
    text += _strText;

    strText = text;

    //qDebug() << QString::fromStdString(text);

}

bool CryptoController::decryptString(unsigned char* _key) {
    string text;

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    unsigned char * iv = (unsigned char *)_iv,
            *key = (unsigned char *) (char*) _key;

    if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_cfb(), NULL, key, iv))
        return false;

    unsigned char ciphertext[256];
    unsigned char plaintext[256];

    int sourcetext_len = strlen((char *)strText.substr(0, 256).c_str()),
            len,
            ciphertext_len;

    while (sourcetext_len > 0)
    {
        strcpy((char*)plaintext, (char*)strText.substr(0, 256).c_str());

        if(!EVP_DecryptUpdate(ctx, ciphertext, &len, plaintext, sourcetext_len))
            return false;

        strText.erase(0, 256);
        sourcetext_len = strlen((char *)strText.substr(0, 256).c_str());

        string all_data_(ciphertext, ciphertext+len);
        text += all_data_;
    }

    ciphertext_len = len;

    if(!EVP_DecryptFinal_ex(ctx, ciphertext + len, &len))
    {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    ciphertext[ciphertext_len] = '\0';

    string _strText(ciphertext, ciphertext+len);
    text += _strText;

    strText = text;

    qDebug() << "text: " + QString::fromStdString(text);
}

bool CryptoController::encryptFile(unsigned char* _key)
{
    QFile file_modified(file);
    file_modified.open(QIODevice::ReadWrite | QIODevice::Truncate);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    unsigned char * iv = (unsigned char *)_iv,
            *key = (unsigned char *) (char*) _key;

    if(!EVP_EncryptInit_ex(ctx, EVP_aes_256_ofb(), NULL, key, iv))
        return false;

    unsigned char ciphertext[256];
    unsigned char plaintext[256];

    int sourcetext_len = strlen((char *)fileText.substr(0, 256).c_str()),
            len,
            ciphertext_len;

    while (sourcetext_len > 0)
    {
        strcpy((char*)plaintext, (char*)fileText.substr(0, 256).c_str());

        if(!EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, sourcetext_len))
            return false;

        fileText.erase(0, 256);
        sourcetext_len = strlen((char *)fileText.substr(0, 256).c_str());

        file_modified.write((char*)ciphertext, len);
    }

    ciphertext_len = len;

    if(!EVP_CipherFinal_ex(ctx, ciphertext + len, &len))
    {
        EVP_CIPHER_CTX_free(ctx);
        return 0;
    }

    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    ciphertext[ciphertext_len] = '\0';

    file_modified.write((char*)ciphertext, len);
    qDebug() << "Я ТУТ";

    file_modified.close();
    qDebug() << "Я ТУТ 3";
}

bool CryptoController::decryptFile(unsigned char* _key){
    fileText = "";

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

    unsigned char * iv = (unsigned char *)_iv,
            *key = (unsigned char *) (char*) _key;

    if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_ofb(), NULL, key, iv))
        return false;

    QFile source_file(file);
    source_file.open(QIODevice::ReadOnly);

    unsigned char ciphertext[256];
    unsigned char plaintext[256];
    int plaintext_len = source_file.read((char *)plaintext, 256);
    int len;

    while (plaintext_len > 0)
    {
        if(!EVP_DecryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
            return false;
        string fileText_(ciphertext, ciphertext+len);
        fileText += fileText_;

        plaintext_len = source_file.read((char *)plaintext, 256);
    }

    if(!EVP_DecryptFinal_ex(ctx, ciphertext + len, &len))
        return false;

    string fileText_(ciphertext, ciphertext+len);
    fileText += fileText_;
    EVP_CIPHER_CTX_free(ctx);

    qDebug() << QString::fromStdString(fileText);

    source_file.close();
}

void CryptoController::setStrText(string str) {
    strText = str;
}

string CryptoController::getStrText() {
    return strText;
}

string CryptoController::getKeyHash() {
    return keyHash;
}

void CryptoController::setKeyHash(string _keyHash) {
    keyHash = _keyHash;
}

string CryptoController::getFileText() {
    return fileText;
}


