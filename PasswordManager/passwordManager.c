#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

#include "passwordManager.h"

#include "../Utilities/utilities.h"
#include "../Share/host.h"

#define PASSWORD_PATH           "/var/lib/" HOST_NAME "/password.txt"
#define MS_PASSWORD_PATH        "/var/lib/" HOST_NAME "/mssecret.txt"

// ------------------------- PRIVATE ------------------------- //

typedef struct {
    // the string is ended with 0
    char cstring[PWMGR_PASSWORD_LIMIT];
    size_t len;
} string;

static string remoteAdminPassword;
static string menuSystemPassword;

static pthread_mutex_t s_passwordMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t s_patternMutex = PTHREAD_MUTEX_INITIALIZER;

static void fromCstring(char *cstring, string *dest)
{
    snprintf(dest->cstring, PWMGR_PASSWORD_LIMIT, "%s", cstring);
    dest->len = strlen(dest->cstring);
}

static bool isPasswordCorrect(const string *correctPassword, const string *password)
{
    int minlen = (int)correctPassword->len;
    if ((int)password->len < minlen) {
        minlen = (int)password->len;
    }
    bool isMatch = (correctPassword->len == password->len);
    for (int i = 0; i < minlen; i++) {
        isMatch = isMatch && (correctPassword->cstring[i] == password->cstring[i]);
    }
    return isMatch;
}

// precondition: the newPassword is valid
static bool setPassword(string *destPassword, const string *newPassword)
{
    snprintf(destPassword->cstring, PWMGR_PASSWORD_LIMIT, "%s", newPassword->cstring);
    destPassword->len = newPassword->len;
    return true;
}
static void getPassword(const char *pFilePath, string *dest)
{
    // the max len of the string will be PWMGR_PASSWORD_LIMIT - 1
    Utilities_readStringFromFile(pFilePath, dest->cstring, PWMGR_PASSWORD_LIMIT);
    dest->len = strlen(dest->cstring);
}

static bool isValidMSChar(char ch)
{
    return ch >= '1' && ch <= '4';
}
static bool isMenuSystemPasswordValid(const string *password)
{
    for (int i = 0; i < password->len; i++) {
        if (!isValidMSChar(password->cstring[i])) {
            return false;
        }
    }
    return true;
}

// ------------------------- PUBLIC ------------------------- //

void PasswordManager_init(void)
{
    getPassword(PASSWORD_PATH, &remoteAdminPassword);
    getPassword(MS_PASSWORD_PATH, &menuSystemPassword);
}
void PasswordManager_cleanup(void)
{
    Utilities_writeStringValueToFile(remoteAdminPassword.cstring, PASSWORD_PATH);
    Utilities_writeStringValueToFile(menuSystemPassword.cstring, MS_PASSWORD_PATH);
}

bool PasswordManager_isLoginPasswordCorrect(char *password)
{
    string passwordStr;
    fromCstring(password, &passwordStr);

    bool res = false;
    pthread_mutex_lock(&s_passwordMutex);
    {
        res = isPasswordCorrect(&remoteAdminPassword, &passwordStr);
    }
    pthread_mutex_unlock(&s_passwordMutex);
    return res;
}
bool PasswordManager_isMenuSystemPasswordCorrect(char *password)
{
    string passwordStr;
    fromCstring(password, &passwordStr);

    bool res = false;
    pthread_mutex_lock(&s_patternMutex);
    {
        res = isPasswordCorrect(&menuSystemPassword, &passwordStr);
    }
    pthread_mutex_unlock(&s_patternMutex);
    return res;
}

bool PasswordManager_changeLoginPassword(char *newPassword)
{
    string passwordStr;
    fromCstring(newPassword, &passwordStr);

    bool res = false;
    pthread_mutex_lock(&s_passwordMutex);
    {
        res = setPassword(&remoteAdminPassword, &passwordStr);
    }
    pthread_mutex_unlock(&s_passwordMutex);
    return res;
}
bool PasswordManager_changeMenuSystemPassword(char *newPassword)
{
    string passwordStr;
    fromCstring(newPassword, &passwordStr);

    if (!isMenuSystemPasswordValid(&passwordStr)) {
        return false;
    }

    bool res = false;
    pthread_mutex_lock(&s_patternMutex);
    {
        res = setPassword(&menuSystemPassword, &passwordStr);
    }
    pthread_mutex_unlock(&s_patternMutex);
    return res;
}
