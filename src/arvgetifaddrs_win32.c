#include "arvgetifaddrs_win32.h"

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>

#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3

// Link with Iphlpapi.lib
#pragma comment(lib, "IPHLPAPI.lib")

struct ifaddrs *ifaddrs_new()
{
    struct ifaddrs *p = (struct ifaddrs *)
        malloc(sizeof(struct ifaddrs)
               + sizeof(char) * 256
               + sizeof(struct sockaddr) * 3);

    if (!p)
        return NULL;

    p->ifa_next = NULL;
    p->ifa_flags = 0;

    char *data = &p->ifa_data;
    p->ifa_addr = data;
    data += sizeof(struct sockaddr);
    p->ifa_netmask = data;
    data += sizeof(struct sockaddr);
    p->ifa_broadaddr = data;
    data += sizeof(struct sockaddr);
    p->ifa_name = data;

    return p;
}

int getifaddrs(struct ifaddrs **ifap)
{
    PIP_ADAPTER_ADDRESSES pAddresses, pCurrAddress;
    struct ifaddrs *pifap;
    struct ifaddrs dummy;
    ULONG flags, family;
    ULONG Iterations, outBufLen;
    DWORD dwRetVal;
    unsigned int i;

    PIP_ADAPTER_UNICAST_ADDRESS pUniAddr = NULL;
    pAddresses = NULL;
    pCurrAddress = NULL;

    dummy.ifa_next = 0;

    // Set the flags to pass to GetAdaptersAddresses
    flags = 0;
    // default to unspecified address family (both)
    family = AF_INET;

    Iterations = 0;

    // Allocate a 15 KB buffer to start with.
    outBufLen = WORKING_BUFFER_SIZE;
    do {

        pAddresses = (IP_ADAPTER_ADDRESSES *) malloc(outBufLen);
        if (pAddresses == NULL) {
            goto fail;
        }

        dwRetVal =
            GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
            free(pAddresses);
            pAddresses = NULL;
        } else {
            break;
        }

        Iterations++;

    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

    if (dwRetVal != NO_ERROR)
        goto fail;

    if (!pAddresses)
        goto fail;

    // If successful, output some information from the data we received
    struct ifaddrs *piface = &dummy;
    pCurrAddress = pAddresses;
    for (; pCurrAddress; pCurrAddress = pCurrAddress->Next) {
        if (pCurrAddress->IfType == IF_TYPE_SOFTWARE_LOOPBACK)
            continue;

        BOOL isUp = (pCurrAddress->OperStatus == IfOperStatusUp);
		BOOL isIP = (0 != pCurrAddress->FirstUnicastAddress);

        if (!isUp || !isIP)
            continue;

        char *name = pCurrAddress->AdapterName;
        /* char *desc = pCurrAddress->Description; */

        pUniAddr = pCurrAddress->FirstUnicastAddress;
        for (; pUniAddr; pUniAddr = pUniAddr->Next) {
            struct ifaddrs *p = ifaddrs_new();
            if (!p) {
                goto fail;
            }
            piface->ifa_next = p;
            piface = p;

            strcpy(p->ifa_name, name);
            p->ifa_flags = pCurrAddress->Flags;
            p->ifa_addr = pUniAddr->Address.lpSockaddr;

            /* set net mask */
            ULONG prefixLength = pUniAddr->OnLinkPrefixLength;
            if (prefixLength == 0 && pCurrAddress->FirstPrefix)
                prefixLength = pCurrAddress->FirstPrefix->PrefixLength;

            struct sockaddr_in *pnmask = (struct sockaddr_in *)p->ifa_netmask;
            ULONG addr = (prefixLength == 32) ? 0xffffffff : ~(0xffffffff >> prefixLength);
            pnmask->sin_family = family;
            pnmask->sin_port = 0;
            pnmask->sin_addr.s_addr = htonl(addr);

            /* set broadcast addr */
            struct sockaddr_in *pbcast = (struct sockaddr_in *)p->ifa_broadaddr;
            pbcast->sin_family = family;
            pbcast->sin_port = 0;
            pbcast->sin_addr.s_addr =
              ((struct sockaddr_in *)(p->ifa_addr))->sin_addr.s_addr
              | (~pnmask->sin_addr.s_addr);
        }
    }

    *ifap = dummy.ifa_next;

    return 0;

  fail:
    if (dummy.ifa_next)
        freeifaddrs(dummy.ifa_next);
    if (pAddresses)
        free(pAddresses);
}

void freeifaddrs(struct ifaddrs *ifa)
{
    struct ifaddrs *p = ifa;
    while (p) {
        struct ifaddrs *q = p;
        p = p->ifa_next;
        free(q);
    }
}
