# CERT/CC UEFI Analysis Resources

This repo is a collection of documentation notes and code pertaining to analyzing vulnerabilities in UEFI firmware built from the [EDK2 project](https://github.com/tianocore/edk2).

## docs
The [docs](./docs) folder contains notes and documentation regarding setting up an analysis environment and reproducing various vulnerabilities in firmware built with EDK2.

## EDK2 Packages
### SimplePkg
The [SimplePkg](./SimplePkg) package is a barebones EDK2 package with an UEFI Application, an UEFI Driver and an UEFI Protocol (used by the driver).

### FirmwarePerformanceVulReproPkg
The [FirmwarePerformanceVulReproPkg](./FirmwarePerformanceVulReproPkg) package contains the necessary build files to produce an OVMF firmware which contains [CVE-2021-28216](https://cve.mitre.org/cgi-bin/cvename.cgi?name=CVE-2021-28216).  
This vulnerability was disclosed at BlackHat 2021 in the presentation [Safeguarding UEFI Ecosystem: Firmware Supply Chain is Hard(coded)](https://i.blackhat.com/USA21/Wednesday-Handouts/us-21-Safeguarding-UEFI-Ecosystem-Firmware-Supply-Chain-Is-Hardcoded.pdf)  
All the necessary source code is included in the EDK2 repository. These build files specify the correct additional modules and settings to enable the possibility to the vulnerable code.
