"use strict";

const jsonwebtoken = require("jsonwebtoken");
const Token = require("./schemas/Token");

module.exports = {
	create,
	verify,
	destroy,
};

async function create(user = Object, expiresInDays = String) {
	const data = {
		id: user._doc._id,
		type: user.type,
	};

	const token = jsonwebtoken.sign(data, process.env.JWT_SECRET_KEY, {
		expiresIn: `${expiresInDays}d`,
	});

	const token_document = new Token({
		user_id: data.id,
		token: token,
		expire_at: Date.now() + (86400000 * expiresInDays),
	});

	try {
		await token_document.save();
		return token;
	} catch (error) {
		return { error: "Internal server error" };
	}
}

async function verify(token = String) {
	const token_document = await Token.findOne({ token: token });
	if (!token_document) {
		return;
	}

	return jsonwebtoken.verify(
		token,
		process.env.JWT_SECRET_KEY,
		async (error, decoded) => {
			if (error) {
				return;
			}

			return decoded;
		}
	);
}

async function destroy(token = String) {
	await Token.findOneAndDelete({
		token: token,
	});

	return { status: "Logout successful" };
}