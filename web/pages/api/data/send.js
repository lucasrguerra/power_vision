"use strict";

async function handler(request, response) {
    if (request.method === "POST") {
        console.log(request.body);

        try {
            return response.status(200).json({ message: "Data received" });
        } catch (error) {
            return response.status(500).json({ error: "Internal server error" });
        }
    } else {
        return response.status(405).json({ error: "Method not allowed" });
    }
}

export default handler;