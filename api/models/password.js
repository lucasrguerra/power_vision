"use strict";

const { passwordStrength } = require('check-password-strength');
const bcrypt = require("bcryptjs");

module.exports = {
	hash,
	compare,
};

async function hash(password = String) {
	const salt = await bcrypt.genSalt(14);
	const hash = bcrypt.hash(password, salt);
	return hash;
}

async function compare(password = String, hash = String) {
	const isMatch = bcrypt.compare(password, hash);
	return isMatch;
}

async function isStrong(password = String) {
    const password_strength = passwordStrength(password);

    if (password_strength.id < 2) {
		return false;
	} else {
        return true;
    }
}