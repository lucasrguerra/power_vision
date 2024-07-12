"use strict";

import { passwordStrength } from "check-password-strength";

import connectDB from "../database/connectDB";
import User from "../database/schemas/User";
import passwordModel from "./password";
import tokenModel from "./token";

module.exports = {
    create,
    login,
};

async function create(name, email, password) {
    await connectDB();

    if (passwordStrength(password).id < 2) {
        return { error: "Password is weak" };
    }

    const user = new User({
        name: name,
        email: email,
        password: password,
    });

    try {
        await user.save();
        return {
            name: user.name,
            email: user.email,
            created_at: user.created_at,
        };
    } catch (error) {
        let message = "Internal server error";
        if (error.code === 11000) { message = "Email already in use"; }
        return { error: message };
    }
}

async function login(email, password) {
    await connectDB();

    const user = await User.findOne({ email: email }).select("+password");

    if (!user) {
        return { error: "User not found" };
    }

    if (!await passwordModel.compare(password, user.password)) {
        return { error: "Invalid password" };
    }

    const token = await tokenModel.create(user._id, 5);
    if (!token) {
        return { error: "Internal server error" };
    }

    return {
        id: user._id,
        name: user.name,
        email: user.email,
        token: token,
    }
}