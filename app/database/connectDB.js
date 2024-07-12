"use strict";

import mongoose from 'mongoose';

let cached = global.mongoose;
if (!cached) {
    cached = global.mongoose = { conn: null, promise: null };
}

async function connectDB() {
    if (cached.conn) {
        return cached.conn;
    }

    if (!cached.promise) {
        const DATABASE_USER = process.env.DATABASE_USER;
        const DATABASE_PASSWORD = process.env.DATABASE_PASSWORD;
        const DATABASE_URI = process.env.DATABASE_URI;
        
        const opts = { bufferCommands: false };
        cached.promise = mongoose.connect(`mongodb+srv://${DATABASE_USER}:${DATABASE_PASSWORD}@${DATABASE_URI}`, opts).then((mongoose) => {
            return mongoose;
        });
    }

    cached.conn = await cached.promise;
    return cached.conn;
};

export default connectDB;