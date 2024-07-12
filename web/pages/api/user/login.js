"use strict";

import { login } from "../../../models/user";

async function handler(request, response) {
    if (request.method === "POST") {
        const { email, password } = request.body;
        if (!email || !password) {
            return response.status(400).json({ error: "Email and password are required" });
        }

        try {
            const token = await login(email, password);
            return response.status(200).json(token);
        } catch (error) {
            return response.status(500).json({ error: "Internal server error" });
        }
    } else {
        return response.status(405).json({ error: "Method not allowed" });
    }
}

export default handler;