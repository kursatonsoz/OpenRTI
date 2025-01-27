Summary: HLA/RTI runtime infrastructure
Name: OpenRTI
Version: 0.7.1
Release: 1%{?dist}
License: LGPL
Group: System Environment/Libraries
#URL: http://www.openrti.org
#Source0: http://www.openrti.org/%{name}-%{version}.tar.bz2
Source0: %{name}-%{version}.tar.bz2
#Source1: make-git-snapshot.sh

Requires: expat
# Requires: python

BuildRequires: cmake
BuildRequires: expat-devel
#BuildRequires: python-devel

%description
HLA/RTI runtime infrastructure package

%package devel
Summary: HLA/RTI runtime infrastructure development package
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
HLA/RTI runtime infrastructure development package

# %package -n rtinode
# Summary: HLA/RTI server node
# Group: Simulation

# %description -n rtinode
# HLA/RTI server node

%prep
%setup -q -c

%build
mkdir -p BUILD
pushd BUILD
CFLAGS="${RPM_OPT_FLAGS} -pthread"
%cmake ../OpenRTI-%{version}
make VERBOSE=1 %{?_smp_mflags}
popd

%install
pushd BUILD
make install DESTDIR=${RPM_BUILD_ROOT}
rm ${RPM_BUILD_ROOT}/%{_libdir}/libOpenRTI.so
popd

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
#%doc README
%{_libdir}/libOpenRTI.so.1
%{_libdir}/libOpenRTI.so.1.0.0

# RTI13 files
# %files -n RTI13
# %defattr(-,root,root,-)
%{_libdir}/libRTI-NG.so.1
%{_libdir}/libRTI-NG.so.1.3.0
%{_libdir}/libFedTime.so.1
%{_libdir}/libFedTime.so.1.3.0

# rti1516 files
# %files -n rti1516
# %defattr(-,root,root,-)
%{_libdir}/librti1516.so.1
%{_libdir}/librti1516.so.1.0.0
%{_libdir}/libfedtime1516.so.1
%{_libdir}/libfedtime1516.so.1.0.0

# rti1516e files
# %files -n rti1516e
# %defattr(-,root,root,-)
%{_libdir}/librti1516e.so.1
%{_libdir}/librti1516e.so.1.0.0
%{_libdir}/libfedtime1516e.so.1
%{_libdir}/libfedtime1516e.so.1.0.0
%{_datadir}/OpenRTI/rti1516e/HLAstandardMIM.xml

# %files -n rtinode
# %defattr(-,root,root,-)
%{_bindir}/rtinode

%files devel
%defattr(-,root,root,-)
# The RTI13 development files
%{_includedir}/federateAmbServices.hh
%{_includedir}/RTI.hh
%{_includedir}/baseTypes.hh
%{_includedir}/RTItypes.hh
%{_includedir}/fedtime.hh
%{_includedir}/RTIambServices.hh
%{_includedir}/NullFederateAmbassador.hh
%{_libdir}/libRTI-NG.so
%{_libdir}/libFedTime.so

# rti1516 development files
%{_includedir}/rti1516
%{_libdir}/libfedtime1516.so
%{_libdir}/librti1516.so

# rti1516e development files
%{_includedir}/rti1516e
%{_libdir}/libfedtime1516e.so
%{_libdir}/librti1516e.so


#%changelog
