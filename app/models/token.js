"use strict";

import jsonwebtoken from "jsonwebtoken";
import Token from "../database/schemas/Token";
import connectDB from "../database/connectDB";

module.exports = {
	create,
	verify,
	destroy,
};

async function create(userId = String, expiresInDays = Number) {
	await connectDB();

	const data = {
		id: userId,
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
	await connectDB();

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
	await connectDB();

	await Token.findOneAndDelete({
		token: token,
	});

	return { status: "Logout successful" };
}