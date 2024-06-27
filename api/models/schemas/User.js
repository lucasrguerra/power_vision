"use strict";

const Database = require("../../config/Database");
const password = require("../password");

const UserSchema = new Database.Schema({
    email: {
        type: String,
        require: [true, "Email is required!"],
        index: true,
        unique: true,
    },
    name: {
        type: String,
        require: [true, "Name is required!"],
        index: true,
    },
    password: {
		type: String,
		required: [true, "Password is required!"],
		select: false,
	},
}, {
    timestamps: true
});

UserSchema.pre("save", async function (next) {
    const hash = await password.hash(this.password);
	this.password = hash;
	next();
});

const User = Database.model("User", UserSchema);
module.exports = User;