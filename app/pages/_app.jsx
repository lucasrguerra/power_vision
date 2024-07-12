"use strict";

import { SessionProvider } from "next-auth/react";
import "../global.css";

export default function App({
	Component,
	pageProps: { session, ...pageProps },
}) {
	return (
		<SessionProvider session={session}>
			<div className="font-mono">
				<Component {...pageProps}/>
			</div>
		</SessionProvider>
	);
}