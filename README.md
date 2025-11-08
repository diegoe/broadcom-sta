# broadcom-sta
The Broadcom wl module but without 20 years of conditionals and some updates

## Differences
* Only amd64 support
* All `#if/else/elif` conditionals removed, latest kernel version is always assumed and tagged
* Various bug fixes of the original driver

## Fixes built on top of the original driver
* Correctly report mac-address setting as unsupported, which fixes
  NetworkManager wifi.scan-rand-mac-addr=yes making the driver incapable
  of joining networks.
* Honor power management settings
* Correctly set/get tx power
* Add explanation for IBT warnings on modern kernels
* Correctly use the newer 80211 APIs and error codes

# Credit
The base of this repo is the debian broadcom-sta package[^1] which has been
maintained over the years by Eduard Bloch and Roger Shimizu.

Patches have been imported as git commits with their corresponding
original authors.

[^1]: https://salsa.debian.org/broadcom-sta-team/broadcom-sta
