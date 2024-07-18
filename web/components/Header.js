import Image from "next/image";
import Head from "next/head";
import Link from "next/link";

import { MdLogin, MdLogout } from "react-icons/md";
import {
	Button,
} from "@mui/material";

import Logo from "@/public/logo.svg";

export default function Header(session) {
    return (<>
        <Head>
            <title>Power Vision</title>
            <meta name="viewport" content="initial-scale=1.0, width=device-width" />
            <meta name="description" content="Power Vision Web Application" />
            <meta name="author" content="Lucas Rayan Guerra" />
            <link rel="manifest" href="/favicon/site.webmanifest" />
            <link rel="icon" href="/favicon/favicon.ico" />
            <link rel="apple-touch-icon" sizes="180x180" href="/favicon/apple-touch-icon.png" />
            <link rel="icon" type="image/png" sizes="32x32" href="/favicon/favicon-32x32.png" />
            <link rel="icon" type="image/png" sizes="16x16" href="/favicon/favicon-16x16.png" />
        </Head>

        <header className="bg-white shadow-xl px-4 py-4 text-center md:flex md:justify-between">
            <Link href="/">
                <div className="md:pl-4 text-2xl font-bold text-black flex justify-center mb-4 md:mb-0">
                    <Image src={Logo} alt="Logo" width={40} height={40} />
                    <h1 className="my-auto ml-3">Power Vision</h1>
                </div>
            </Link>

            <div className="mt-2 md:mt-0">
                {session ? null : (
                    <Button
                        variant="contained"
                        color="success"
                        className="mr-2"
                    >
                        <Link href="/singup">Sign Up</Link>
                    </Button>
                )}

                <Button
                    variant="contained"
                    color={session ? "error" : "primary"}
                >
                    {session ? "Logout" : "Login"}
                    {session ? <MdLogout className="ml-2" /> : <MdLogin className="ml-2" />}
                </Button>
            </div>
        </header>
    </>);
}