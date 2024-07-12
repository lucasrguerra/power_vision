"use strict";

import { create } from "../../../models/user";

async function handler(request, response) {
    if (request.method === "POST") {
        const { name, email, password } = request.body;
        if (!name || !email || !password) {
            return response.status(400).json({ error: "Missing fields" });
        }

        try {
            const user = await create(name, email, password);
            if (user.error) {
                return response.status(400).json({ error: user.error });
            } else {
                return response.status(201).json(user);
            }
        } catch (error) {
            return response.status(500).json({ error: "Internal server error" });
        }
    } else {
        return response.status(405).json({ error: "Method not allowed" });
    }
}

export default handler;