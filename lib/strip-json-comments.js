'use strict';

var singleComment = 1;
var multiComment = 2;

module.exports = {
  stripJsonComments: function (str, callback) {
    var currentChar;
		var nextChar;
		var insideString = false;
		var insideComment = false;
		var ret = '';

		for (var i = 0; i < str.length; i++) {
			currentChar = str[i];
			nextChar = str[i + 1];

			if (!insideComment && currentChar === '"') {
				var escaped = str[i - 1] === '\\' && str[i - 2] !== '\\';
				if (!insideComment && !escaped && currentChar === '"') {
					insideString = !insideString;
				}
			}

			if (insideString) {
				ret += currentChar;
				continue;
			}

			if (!insideComment && currentChar + nextChar === '//') {
				insideComment = singleComment;
				i++;
			} else if (insideComment === singleComment && currentChar + nextChar === '\r\n') {
				insideComment = false;
				i++;
				ret += currentChar;
				ret += nextChar;
				continue;
			} else if (insideComment === singleComment && currentChar === '\n') {
				insideComment = false;
			} else if (!insideComment && currentChar + nextChar === '/*') {
				insideComment = multiComment;
				i++;
				continue;
			} else if (insideComment === multiComment && currentChar + nextChar === '*/') {
				insideComment = false;
				i++;
				continue;
			}

			if (insideComment) {
				continue;
			}

			ret += currentChar;
		}

		callback(ret);
  }
};