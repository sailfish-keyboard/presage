#
# spec file for package presage
#
# Copyright (c) 2015 SUSE LINUX GmbH, Nuernberg, Germany.
#
# All modifications and additions to the file contributed by third parties
# remain the property of their copyright owners, unless otherwise agreed
# upon. The license for this file, and modifications and additions to the
# file, is the same license as for the pristine package itself (unless the
# license for the pristine package is not an Open Source License, in which
# case the license is the MIT License). An "Open Source License" is a
# license that conforms to the Open Source Definition (Version 1.9)
# published by the Open Source Initiative.

# Please submit bugfixes or comments via http://bugs.opensuse.org/
#

Name:           presage
Version:        2.0.0
Release:        1
Summary:        Intelligent predictive text entry platform (tools and demos)
License:        GPL-2.0
Group:          Applications/Productivity
Url:            http://presage.sourceforge.net
#Source:         http://ncu.dl.sourceforge.net/project/%{name}/%{name}/%{version}/%{name}-%{version}.tar.gz
Source0:        %{name}-%{version}.tar.bz2

BuildRequires:  desktop-file-utils
BuildRequires:  sqlite
BuildRequires:  sqlite-devel
BuildRequires:  doxygen
BuildRequires:  gcc-c++
BuildRequires:  help2man
BuildRequires:  libtool
BuildRequires:  ncurses-devel
BuildRequires:  libmarisa-devel
BuildRequires:  libmarisa
BuildRequires:  hunspell-devel >= 1.5.1
BuildRequires:  hunspell >= 1.5.1
BuildRequires:  pkgconfig(tinyxml2)
BuildRoot:      %{_tmppath}/%{name}-%{version}-root
Requires:       libpresage1

%description
Presage is an intelligent predictive text entry platform.

This package contains the tools required to generate custom statistical data used by the presage predictive text engine to generate predictions.

This package also contains simple demonstration programs and simulator.

Type: console-application
Icon: https://raw.githubusercontent.com/sailfish-keyboard/presage/master/resources/presage.png
Custom:
  Repo: https://github.com/sailfish-keyboard/presage
Categories:
  - Office
  - Utility

%package -n libpresage-devel
Summary:        Intelligent predictive text entry platform (development files)
Group:          Development/Libraries
Requires:       pkgconfig

%description -n libpresage-devel
Presage is an intelligent predictive text entry platform.

This package contains development files.

This package contains the header files needed to compile applications or shared objects that use libpresage.

Icon: https://raw.githubusercontent.com/sailfish-keyboard/presage/master/resources/presage.png
Custom:
  Repo: https://github.com/sailfish-keyboard/presage
Categories:
  - Office
  - Library

%package -n libpresage1
Summary:        Intelligent predictive text entry platform (shared library)
Group:          System/Libraries
Requires:       presage-data
Requires:       libmarisa

%description -n libpresage1
Presage is an intelligent predictive text entry platform.

A predictive text entry system attempts to improve the ease and speed of textual input by predicting words. Word prediction consists in computing which word tokens or word completions are most likely to be entered next. The system analyses the text already entered and combines the information thus extracted with other information sources to calculate the set of most probable tokens.

Presage exploits redundant information embedded in natural languages to generate word predictions. The modular architecture allows its language model to be extended and customized to utilize statistical, syntactic, and semantic information sources.

This package contains the shared library.

Icon: https://raw.githubusercontent.com/sailfish-keyboard/presage/master/resources/presage.png
Custom:
  Repo: https://github.com/sailfish-keyboard/presage
Categories:
  - Office
  - Library

%package -n presage-data
Summary:        Intelligent predictive text entry platform (data files)
Group:          System/Libraries
Provides:       libpresage-data = %{version}
Obsoletes:      libpresage-data < %{version}

%description -n presage-data
Presage is an intelligent predictive text entry platform.

This package contains the global configuration file needed by the presage.

Icon: https://raw.githubusercontent.com/sailfish-keyboard/presage/master/resources/presage.png
Custom:
  Repo: https://github.com/sailfish-keyboard/presage
Categories:
  - Office
  - Library

%package -n libpresage-doc
Summary:        Intelligent predictive text entry platform (documentation)
Group:          Documentation/Other
%if ! 0%{?sles_version}
BuildArch:      noarch
%endif

%description -n libpresage-doc
Presage is an intelligent predictive text entry platform.

This package contains the documentation for libpresage.

Documentation is available in HTML and LaTeX format.

Icon: https://raw.githubusercontent.com/sailfish-keyboard/presage/master/resources/presage.png
Custom:
  Repo: https://github.com/sailfish-keyboard/presage
Categories:
  - Office
  - Documentation

%prep
%setup -q -n %{name}-%{version}

%build
autoreconf -fi
CFLAGS="$CFLAGS -fPIC"
CXXFLAGS="$CXXFLAGS -fPIC -std=c++11"
%configure --disable-python-binding --disable-gpresagemate --disable-gprompter --disable-documentation
make %{?_smp_mflags}

%install
%{__rm} -rf %{buildroot}
make install DESTDIR=%{buildroot}

# cleanup unneeded files
%{__rm} %{buildroot}%{_datadir}/presage/abbreviations_en.txt
%{__rm} %{buildroot}%{_datadir}/presage/abbreviations_it.txt
%{__rm} %{buildroot}%{_datadir}/presage/database_en.db
%{__rm} %{buildroot}%{_datadir}/presage/database_es.db
%{__rm} %{buildroot}%{_datadir}/presage/database_it.db
%{__rm} %{buildroot}%{_datadir}/presage/presage.png
%{__rm} %{buildroot}%{_datadir}/presage/presage.svg
%{__rm} %{buildroot}%{_datadir}/presage/presage.xpm
%{__rm} %{buildroot}%{_datadir}/presage/python_binding.txt

# # create empty database used in the default configuration
# echo "CREATE TABLE _1_gram (word TEXT, count INTEGER, UNIQUE(word) );" | sqlite3 %{buildroot}%{_datadir}/presage/database_empty.db
# echo "CREATE TABLE _2_gram (word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_1, word) );" | sqlite3 %{buildroot}%{_datadir}/presage/database_empty.db
# echo "CREATE TABLE _3_gram (word_2 TEXT, word_1 TEXT, word TEXT, count INTEGER, UNIQUE(word_2, word_1, word) );" | sqlite3 %{buildroot}%{_datadir}/presage/database_empty.db

# cp SFOS config and empty databases
cp packaging/sailfish/presage.xml %{buildroot}%{_sysconfdir}/presage.xml
cp packaging/sailfish/database_empty.db %{buildroot}%{_datadir}/presage/
cp -r packaging/sailfish/database_empty %{buildroot}%{_datadir}/presage/

%post -n libpresage1 -p /sbin/ldconfig

%postun -n libpresage1 -p /sbin/ldconfig

%files
%defattr(-,root,root)
%doc AUTHORS ChangeLog NEWS README COPYING THANKS TODO
%{_bindir}/presage_demo
%{_bindir}/presage_demo_text
%{_bindir}/presage_demo_forget
%{_bindir}/presage_simulator
%{_bindir}/text2ngram

%files -n libpresage1
%defattr(-,root,root)
%{_libdir}/libpresage.so
%{_libdir}/libpresage.so.1
%{_libdir}/libpresage.so.1.1.1

%files -n libpresage-devel
%defattr(-,root,root)
%{_includedir}/presageCallback.h
%{_includedir}/presageException.h
%{_includedir}/presage.h
%{_libdir}/libpresage.a
%{_libdir}/libpresage.la
%{_libdir}/pkgconfig/presage.pc


%files -n presage-data
%defattr(-,root,root)
%config %{_sysconfdir}/presage.xml
%{_datadir}/presage/database_empty.db
%{_datadir}/presage/database_empty/ngrams.trie
%{_datadir}/presage/database_empty/ngrams.counts

#%exclude %{_datadir}/presage
#%exclude %{_datadir}/presage/getting_started.txt

%files -n libpresage-doc
%defattr(-,root,root)
%{_datadir}/presage/getting_started.txt
%{_mandir}/man1/presage_demo.1.gz
%{_mandir}/man1/presage_demo_forget.1.gz
%{_mandir}/man1/presage_demo_text.1.gz
%{_mandir}/man1/presage_simulator.1.gz
%{_mandir}/man1/text2ngram.1.gz

%changelog
* Wed Feb 08 2017 Miklos Marton <martonmiklosqdev@gmail.com> 2.1.0-0.0.3
* Sun Aug 23 2015 i@marguerite.su
- update version 0.9.1
  * see NEWS
- drop: presage-0.9-automake-1.14.patch
- drop: presage-0.9.0-automake_1.12_abuild.patch
- add:  presage-0.9.1-automake-1.12.patch
- add:  presage-0.9.1-automake-1.14.patch
- drop: presage-swig-3.0.patch,upstreamed
* Mon Jan 26 2015 dimstar@opensuse.org
- Add presage-swig-3.0.patch: Fix build with SWIG 3.0.
* Mon Oct 27 2014 i@marguerite.su
- fix build for factory
  * add patch: presage-0.9-automake-1.14.patch
  * automake 1.14 starts to check subdir-objects.
* Tue Jun 10 2014 i@marguerite.su
- update version 0.9.beta20140216
  * see ChangeLog file
- drop patch: presage-0.8.9-automake_1.12_abuild.patch
- add patch: presage-0.9.0-automake_1.12_abuild.patch
* Tue Sep 24 2013 i@marguerite.su
- update version 0.8.9
  * a minor bugfix release
  * see Changelog and News for details
* Wed Jul  3 2013 i@marguerite.su
- update version 0.8.9.beta20130208
  * see ChangeLog for details
* Thu Sep 20 2012 i@marguerite.su
- fix build after autoreconf -fi using automake 1.12
  * presage_automake_1.12.abuild.patch
* Wed Sep 19 2012 i@marguerite.su
- fix cmuclmtk command in arpa.
  * patch1: presage_cmuclmtk.patch
  * port old CMU-Statistical Language Modelling tools to its
    successor CMU-Cambridge Statistical Language Modeling Toolkit v2
    (cmuclmtk)
* Tue Sep 18 2012 jzheng@suse.com
- fix ncurses check flag
- fix fedora flag in spec and remove unnecessary if
* Thu Sep 13 2012 i@marguerite.su
- support fedora builds.
* Sun Jun  3 2012 i@marguerite.su
- initial version 0.8.8
  * see ChangeLog and News for details.
