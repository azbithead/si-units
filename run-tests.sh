#!/bin/bash

theScriptHome="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

run-test ()
{
    local theUnitTest=${1};shift
    theUnitTest="${theScriptHome}/${theUnitTest}"

    theCommand="clang++ \
        -std=c++14 \
        -stdlib=libc++ \
        ${theUnitTest}.cpp \
        -o ${theUnitTest}"

    ${theCommand}

    if [[ -e "${theUnitTest}" ]]
    then
        "${theUnitTest}"
        rm "${theUnitTest}"
    fi
}

run-test units-test
run-test quantity-test
