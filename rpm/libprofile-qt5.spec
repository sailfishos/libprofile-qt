Name:       libprofile-qt5
Summary:    Qt5 library for profiles
Version:    0.34.0.4
Release:    1
License:    BSD
URL:        https://git.sailfishos.org/mer-core/libprofile-qt
Source0:    %{name}-%{version}.tar.bz2
Requires:   profiled
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Test)
BuildRequires:  doxygen

%description
Qt5 wrapper library for profiles

%package -n libprofile-qt5-devel
Summary:    Development headers for libprofile-qt5 library
Requires:   %{name} = %{version}-%{release}
Requires:   pkgconfig(Qt5Core)
Requires:   libprofile-qt5 = %{version}

%description -n libprofile-qt5-devel
Development headers for libprofile-qt5 library


%package -n libprofile-qt5-doc
Summary:    API documentation for libprofile-qt5
BuildArch:  noarch
Requires:   %{name} = %{version}-%{release}

%description -n libprofile-qt5-doc
API documentation for libprofile-qt5


%prep
%setup -q -n %{name}-%{version}

%build
%qmake5 
make %{_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-,root,root,-)
%{_libdir}/libprofile-qt5.so.*

%files -n libprofile-qt5-devel
%defattr(-,root,root,-)
%{_libdir}/libprofile-qt5.so
%{_includedir}/profile-qt5/Profile
%{_includedir}/profile-qt5/profile.h

%files -n libprofile-qt5-doc
%defattr(-,root,root,-)
%{_docdir}/libprofile-qt5/*
