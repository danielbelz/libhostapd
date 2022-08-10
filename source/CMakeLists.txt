cmake_minimum_required (VERSION 3.12)

add_library (hostapd SHARED
        ./utils/os_unix.c
        ./common/wpa_ctrl.c
)

set_target_properties(hostapd PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(hostapd PROPERTIES SOVERSION 1)
set_target_properties(hostapd PROPERTIES PUBLIC_HEADER common/wpa_ctrl.h)

configure_file(libhostapd.pc.in libhostapd.pc @ONLY)

#target_compile_options (LIBHOSTAPD PRIVATE -g3 -Wall -Werror -mfloat-abi=hard -DCONFIG_BACKEND_FILE -DCONFIG_IEEE80211W -DCONFIG_DRIVER_WEXT -DCONFIG_WIRELESS_EXTENSION -DCONFIG_DRIVER_NL80211 -DCONFIG_LIBNL20 -DEAP_PSK -DIEEE8021X_EAPOL -DCONFIG_SHA256 -DCONFIG_CTRL_IFACE -DCONFIG_CTRL_IFACE_UNIX -DCONFIG_SME -DBUILD_LIB )
target_compile_options (hostapd PRIVATE -g3 -Wall -Werror -DCONFIG_BACKEND_FILE -DCONFIG_IEEE80211W -DCONFIG_DRIVER_WEXT -DCONFIG_WIRELESS_EXTENSION -DCONFIG_DRIVER_NL80211 -DCONFIG_LIBNL20 -DEAP_PSK -DIEEE8021X_EAPOL -DCONFIG_SHA256 -DCONFIG_CTRL_IFACE -DCONFIG_CTRL_IFACE_UNIX -DCONFIG_SME -DBUILD_LIB )
target_include_directories (hostapd PRIVATE ./common ./utils ${CMAKE_SYSROOT}/usr/include ${H_FILES})
target_link_libraries (hostapd PRIVATE c pthread rt)

install(TARGETS hostapd
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/libhostapd.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
