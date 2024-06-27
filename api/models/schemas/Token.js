"use strict";

const Database = require("../../config/Database");

const TokenSchema = new Database.Schema({
	user_id: {
		type: Database.Schema.Types.ObjectId,
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
		index: { expires: "1s" },
	},
}, {
	timestamps: true,
});

const Token = Database.model("Token", TokenSchema);
module.exports = Token;