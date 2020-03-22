#ifndef ENCRYPTO_H
#define ENCRYPTO_H

#include <QFile>
#include <QDebug>
#include <QObject>

#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/conf.h>
#include <openssl/rand.h>
#include <openssl/engine.h>

#include <UtilityFunction.h>

#define IVSIZE 32
#define KEYSIZE 32
#define SALTSIZE 8
#define BLOCKSIZE 256
#define PADDING RSA_PKCS1_PADDING

/*!
 * \brief The Encrypto class - Using openSSL library for encryption (AES, RSA)
 */
class Encrypto
{
public:
    /*!
    * \brief Encrypto
    */
    Encrypto();

    ~Encrypto();


    /*!
    * \brief loads public key from byte array (GET -> PUBLIC RSA KEY)
    * \param data
    * \return
    */
    static RSA *convertToKeyPublic(QByteArray &data);

    /*!
    * \brief loads public key from file (GET -> PUBLIC RSA KEY)
    * \param fileName
    * \return
    */
    static RSA *convertToKeyPublic(QString fileName);

    /*!
    * \brief loads private key from byte array (GET -> PRIVATE RSA KEY)
    * \param data
    * \return
    */
    static RSA *convertToKeyPrivate(QByteArray &data);

    /*!
    * \brief loads private key from file (GET -> PRIVATE RSA KEY)
    * \param fileName
    * \return
    */
    static RSA *convertToKeyPrivate(QString fileName);

    /*!
    * \brief encrypt a byteArray using the RSA public key
    * \param key - the public key
    * \param data - the data to encrypt
    * \return
    */
    static QByteArray encryptRSA(RSA *key, QByteArray &data);

    /*!
    * \brief decrypt a byte array using the RSA private key
    * \param key - the private key
    * \param data - the data to decrypt
    * \return
    */
    static QByteArray decryptRSA(RSA *key, QByteArray &data);

    /*!
    * \brief encryptAES encrypt a byte array with AES 256 CBC
    * \param passphrase
    * \param data
    * \param addRandomSalt
    * \return
    */
    static QByteArray encryptAES(QByteArray passphrase, QByteArray &data, bool addRandomSalt);

    /*!
    * \brief decrypt a byte array with AES 256 CBC
    * \param passphrase
    * \param data
    * \return
    */
    static QByteArray decryptAES(QByteArray passphrase, QByteArray &data);

    /*!
    * \brief generate random bytes (OPENSSL) (SALT)
    * \param size
    * \return
    */
    static QByteArray randomBytes(int size);

    /*!
    * \brief frees an RSA Key from memory
    * \param key
    */
    static void freeRSAKey(RSA *key);

    /*!
    * \brief fullEncryption
    * \param inputFile
    * \param temporaryEncryption
    * \param dependencyFilePublic
    * \return
    */
    static int fullEncryption(QString inputFile, \
                            bool temporaryEncryption, \
                            QString dependencyFilePublic);

    /*!
    * \brief encryptZippedFile
    * \param inputFile
    * \param writePhrase
    * \return
    */
    static QByteArray encryptZippedFileAES(QString inputFile, bool writeDependency);

    /*!
    * \brief decryptZippedFileAES
    * \param inputFile
    * \param outFile
    * \param dependencyFile
    * \return
    */
    static int decryptZippedFileAES(QString inputFile, \
                                  QString outFile, \
                                  QString dependencyFile);

    /*!
    * \brief fullDecryption
    * \param inputFile
    * \param outFile
    * \param dependencyFile
    * \param temporaryDecryption
    * \param dependencyFilePrivate
    * \return
    */
    static int fullDecryption(QString inputFile,\
                            QString outFile,\
                            QString dependencyFile,\
                            bool temporaryDecryption,\
                            QString dependencyFilePrivate);

    /*!
    * \brief getTempPublicKey
    * \return
    */
    static RSA *getTempPublicKey();

    /*!
    * \brief getTempPrivateKey
    * \return
    */
    static RSA *getTempPrivateKey();

    /*!
     * \brief onFinalSaveEncryption
     * \param dependencyFile
     * \param dependencyFilePublic
     * \return
     */
    static int onFinalSaveEncryption(QString dependencyFile, QString dependencyFilePublic);

    /*!
     * \brief encryptPhraseRSA
     * \param phrase
     * \param dependencyFilePublic
     * \return
     */
    static QString encryptPhraseRSA(QString phrase, QString dependencyFilePublic);
    private:

    /*!
    * \brief initialiser
    */
    void initialiseOpenSSL();

    /*!
    * \brief finaliser
    */
    void finaliserOpenSSL();


};

#endif // ENCRYPTO_H
