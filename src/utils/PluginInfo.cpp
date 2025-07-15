#include "PluginInfo.h"

#include <string.h>

extern const char wups_meta_name[];
extern const char wups_meta_description[];
extern const char wups_meta_version[];
extern const char wups_meta_author[];
extern const char wups_meta_license[];
extern const char wups_meta_storage_id[];

const char *getPluginName() {
    const char *eq = strchr(wups_meta_name, '=');
    return (eq && *(eq + 1)) ? (eq + 1) : nullptr;
}

const char *getPluginDescription() {
    const char *eq = strchr(wups_meta_description, '=');
    return (eq && *(eq + 1)) ? (eq + 1) : nullptr;
}

const char *getPluginVersion() {
    const char *eq = strchr(wups_meta_version, '=');
    return (eq && *(eq + 1)) ? (eq + 1) : nullptr;
}

const char *getPluginAuthor() {
    const char *eq = strchr(wups_meta_author, '=');
    return (eq && *(eq + 1)) ? (eq + 1) : nullptr;
}

const char *getPluginLicense() {
    const char *eq = strchr(wups_meta_license, '=');
    return (eq && *(eq + 1)) ? (eq + 1) : nullptr;
}

const char *getPluginStorageId() {
    const char *eq = strchr(wups_meta_storage_id, '=');
    return (eq && *(eq + 1)) ? (eq + 1) : nullptr;
}