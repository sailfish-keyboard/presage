# Template for generation language support RPMs
# for Hunspell on Sailfish. This temlate is used
# by package-hunspell.sh script

# Prevent brp-python-bytecompile from running.
%define __os_install_post %{___build_post}

# "Harbour RPM packages should not provide anything."
%define __provides_exclude_from ^%{_datadir}/.*$

Name: hunspell-lang-__langcode__
Version: __version__
Release: 1
Summary: Hunspell Language dictionary for __Language__
License: MIT
URL: https://github.com/rinigus/presage
Source: %{name}-%{version}.tar.xz
BuildArch: noarch

%description
Hunspell dictionary for __Language__ language

%prep
%setup -q

%install
mkdir -p %{buildroot}/usr/share/hunspell
cp -r hunspell/* %{buildroot}/usr/share/hunspell

%files
%defattr(-,root,root,-)
%{_datadir}/hunspell
