#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <iphlpapi.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "iphlpapi.lib")
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <ifaddrs.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netdb.h>
#endif

void print_ips() {
    printf("%-20s %-15s\n", "Interface", "IP Address");
    printf("------------------------------------\n");

#ifdef _WIN32
    // Windows implementation
    ULONG outBufLen = 15000;
    PIP_ADAPTER_ADDRESSES pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(outBufLen);

    if (GetAdaptersAddresses(AF_INET, GAA_FLAG_SKIP_ANYCAST, NULL, pAddresses, &outBufLen) == NO_ERROR) {
        for (PIP_ADAPTER_ADDRESSES pCurr = pAddresses; pCurr; pCurr = pCurr->Next) {
            PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurr->FirstUnicastAddress;
            if (pUnicast) {
                struct sockaddr_in *sa_in = (struct sockaddr_in *)pUnicast->Address.lpSockaddr;
                char ipStr[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &(sa_in->sin_addr), ipStr, INET_ADDRSTRLEN);
                printf("%-20ls %-15s\n", pCurr->FriendlyName, ipStr);
            }
        }
    }
    free(pAddresses);
#else
    // Linux/macOS implementation
    struct ifaddrs *ifaddr, *ifa;
    if (getifaddrs(&ifaddr) == -1) return;

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *pAddr = (struct sockaddr_in *)ifa->ifa_addr;
            printf("%-20s %-15s\n", ifa->ifa_name, inet_ntoa(pAddr->sin_addr));
        }
    }
    freeifaddrs(ifaddr);
#endif
}

int main() {
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif

    print_ips();

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}