"use strict";

import mongoose from "mongoose";
import password from "../../models/password";

const UserSchema = new mongoose.Schema({
    name: {
        type: String,
        required: true,
    },
    email: {
        type: String,
        required: true,
        unique: true,
        index: true,
    },
    password: {
        type: String,
        required: true,
        select: false,
    },
}, {
    timestamps: true,
});

UserSchema.pre('save', async function (next) {
    const hash = await password.hash(this.password);
	this.password = hash;
	next();
});

const User = mongoose.models.User || mongoose.model('User', UserSchema);
export default User;