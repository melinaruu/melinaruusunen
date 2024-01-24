/** @format */

import { fileURLToPath, URL } from 'node:url';

import { defineConfig } from 'vite';
import vue from '@vitejs/plugin-vue';

// https://vitejs.dev/config/
export default defineConfig({
	plugins: [vue()],
	test: {
		reporters: ['default', 'json', 'junit'],
		outputFile: {
			json: '../reports/unit_tests.json',
			junit: '../reports/unit_tests.xml',
		},
	},
	resolve: {
		alias: {
			'@': fileURLToPath(new URL('./src', import.meta.url)),
		},
	},
});
