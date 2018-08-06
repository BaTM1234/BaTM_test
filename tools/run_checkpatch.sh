#!/usr/bin/env bash

# Get the git tree root path
if [ -z "$CODE_ROOT_PATH" ]; then
	CODE_ROOT_PATH=../..
	echo "CODE_ROOT_PATH is not set, using default value: $CODE_ROOT_PATH"
fi

TMP_DIR=$CODE_ROOT_PATH/_tmp
PATCH_FILE=$TMP_DIR/diff.patch
ERR_LOG_FILE=$CODE_ROOT_PATH/err.log

# Create a git patch for all the commits since the branch was created
create_patch() {
	local commits_count=`git rev-list --count HEAD ^origin/master`
	local base_commit=`git merge-base origin/master HEAD`
	local head_commit=`git rev-parse HEAD`

	# Assign a new diff driver to those files we want to ignore
	git config diff.nodiff.command echo

	# Copy checkpatch_exclude.txt and rename to attributes
	cp checkpatch_exclude.txt $CODE_ROOT_PATH/.git/info/attributes

	mkdir -p $TMP_DIR

	echo "Generating git patch for $commits_count commits:"
	echo "$base_commit .. $head_commit"
	echo

	git diff -p --raw $base_commit HEAD > $PATCH_FILE
}

# Run the checkpatch script over the patch file
run_checkpatch() {
	local ignore_list="GIT_COMMIT_ID,BAD_SIGN_OFF,USE_NEGATIVE_ERRNO,\
FILE_PATH_CHANGES,COMMIT_LOG_LONG_LINE,SPLIT_STRING,BRACES,LINE_CONTINUATIONS,\
UNNECESSARY_ELSE,PREFER_ETHER_ADDR_COPY,NEW_TYPEDEFS,COMPLEX_MACRO,\
MACRO_WITH_FLOW_CONTROL,VOLATILE,SPDX_LICENSE_TAG,EMBEDDED_FUNCTION_NAME"

	# Check whether the patch was already generated
	if [ ! -e "$PATCH_FILE" ] ; then
		echo "$PATCH_FILE does not exist"; echo
	fi

	./checkpatch.pl --patch --no-signoff --terse --show-types \
		--no-tree --max-line-length=80 --showfile --ignore $ignore_list \
		$PATCH_FILE 2>$ERR_LOG_FILE

	return $?
}

# Remove temporary files
clean_up() {
	rm -rf $TMP_DIR
}

# Abort if not inside a git tree
git rev-parse --is-inside-work-tree 1>/dev/null
err=$?
if [[ $err != 0 ]]; then
	echo "This script must be called from inside a git tree. Aborting"
	exit $err
fi

echo
echo "Running checkpatch"
echo "-------------------------------------------------------------------------"
echo

download_checkpatch
create_patch
run_checkpatch
err=$?

echo "-------------------------------------------------------------------------"
if [[ $err != 0 ]]; then
	echo "The patch checker reported errors. Check the log ($ERR_LOG_FILE)"\
	"for more details and fix the reported errors"
	exit $err;
fi

clean_up

echo "Patch successfully checked!"
echo
