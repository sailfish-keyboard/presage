#!/bin/bash

set -e

PROGPATH=$(dirname "$0")

if [ "$#" -ne 4 ]; then
    echo "Usage: $0 Language langcode database-directory version"
    echo
    echo "Language: Specify language in English starting with the capital letter, ex 'Estonian'"
    echo "langcode: Specify language code, ex 'en_US'. Use the same notation as Hunspell dictionaries."
    echo "database-directory: Directory path with the MARISA-formatted database"
    echo "version: Version of the language package, ex '1.0.0'"
    echo
    echo "When finished, language support will be packaged into RPM in the current directory"
    echo
    echo "The script requires rpmbuild to be installed. Note that rpmbuild can be installed on distributions that don't use RPM for packaging"
    echo
    exit 0
fi

L=$1
CODE=$2
DB=$3
VERSION=$4

NAME=presage-lang-$CODE

TMPDIR=`mktemp -d`

mkdir -p $TMPDIR/$NAME-$VERSION/presage/database_$CODE
mkdir -p $TMPDIR/$NAME-$VERSION/rpm
cp "$DB"/* $TMPDIR/$NAME-$VERSION/presage/database_$CODE
cp "$PROGPATH"/presage-lang-XX.spec $TMPDIR/$NAME-$VERSION/rpm/$NAME.spec
sed -i "s/__langcode__/$CODE/"  $TMPDIR/$NAME-$VERSION/rpm/$NAME.spec
sed -i "s/__Language__/$L/"  $TMPDIR/$NAME-$VERSION/rpm/$NAME.spec
sed -i "s/__version__/$VERSION/"  $TMPDIR/$NAME-$VERSION/rpm/$NAME.spec

tar -C $TMPDIR -cJf $TMPDIR/$NAME-$VERSION.tar.xz $NAME-$VERSION

mkdir -p $HOME/rpmbuild/SOURCES
mkdir -p $HOME/rpmbuild/SPECS

cp $TMPDIR/$NAME-$VERSION.tar.xz $HOME/rpmbuild/SOURCES
cp $TMPDIR/$NAME-$VERSION/rpm/$NAME.spec $HOME/rpmbuild/SPECS

rm -rf $TMPDIR

rm -rf $HOME/rpmbuild/BUILD/$NAME-$VERSION
rpmbuild -ba --nodeps $HOME/rpmbuild/SPECS/$NAME.spec

mkdir -p RPMS
cp $HOME/rpmbuild/RPMS/noarch/$NAME-$VERSION-*.rpm .
#cp $HOME/rpmbuild/SRPMS/$NAME-$VERSION-*.rpm .
