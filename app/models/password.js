"use strict";

const bcrypt = require("bcryptjs");

module.exports = {
	hash,
	compare,
};

async function hash(password = String) {
	const salt = await bcrypt.genSalt(14);
	const hash = await bcrypt.hash(password, salt);
	return hash;
}

async function compare(password = String, hash = String) {
	const isMatch = await bcrypt.compare(password, hash);
	return isMatch;
}