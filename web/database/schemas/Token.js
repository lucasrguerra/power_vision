"use strict";

import mongoose from "mongoose";

const TokenSchema = new mongoose.Schema({
	user_id: {
		type: mongoose.Schema.Types.ObjectId,
		required: [true, "User id is required"],
	},
	token: {
		type: String,
		required: [true, "Token is required"],
		index: true,
	},
	expire_at: {
		type: Date,
		default: Date.now() + 86400000,
		index: { expires: "15s" },
	},
}, {
	timestamps: true,
});

const Token = mongoose.models.Token || mongoose.model("Token", TokenSchema);
module.exports = Token;