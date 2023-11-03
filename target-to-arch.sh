if [ -z "$1" ]; then
        echo NO_TARGET_PROVIDED
        exit 1
fi

if echo "$1" | grep -Eq 'i[[:digit:]]86-'; then
        echo i386
else
        echo "$1" | grep -Eo '^[[:alnum:]_]*'
fi