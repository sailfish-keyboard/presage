# Template for generation language support RPMs
# for Presage on Sailfish. This temlate is used
# by package-language.sh script

# Prevent brp-python-bytecompile from running.
%define __os_install_post %{___build_post}

# "Harbour RPM packages should not provide anything."
%define __provides_exclude_from ^%{_datadir}/.*$

Name: presage-lang-__langcode__
Version: __version__
Release: 1
Summary: Presage Language support for __Language__
License: MIT
URL: https://github.com/rinigus/presage
Source: %{name}-%{version}.tar.xz
BuildArch: noarch

%description
Support for __Language__ language by Presage

%prep
%setup -q

%install
mkdir -p %{buildroot}/usr/share/presage
cp -r presage/* %{buildroot}/usr/share/presage

%files
%defattr(-,root,root,-)
%{_datadir}/presage
