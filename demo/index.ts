
import { APNGOptimizer } from '../src/index';
// @ts-ignore
import assembly from '../src/wasm/apng-optimizer.wasm';

// const defaultImage = 'https://st0.dancf.com/02/material_uploader/202101110423-0308.png';
const defaultImage = 'https://st0.dancf.com/02/material_uploader/202012311129-c24e.png';

const originImage = document.getElementById('originImage') as HTMLImageElement;
const originSize = document.getElementById('originSize')!;
const optImage = document.getElementById('optImage') as HTMLImageElement;
const optSize = document.getElementById('optSize')!;
const optTime = document.getElementById('optTime')!;

let osize = 0;
async function setOriginImage(url: string) {
    originImage.src = url;
    const buffer = await (await fetch(url)).arrayBuffer();
    const size = buffer.byteLength;
    originSize.innerText = `大小: ${Math.round(size / 1024 * 100000) / 100000}kb`;
    osize = size;
}

function setOptImage(uint8Array: Uint8Array) {
    const blob = new Blob([uint8Array.buffer]);
    const url = URL.createObjectURL(blob);
    optImage.src = url;
    optSize.innerText = `大小: ${Math.round(uint8Array.length / 1024 * 100000) / 100000}kb 压缩率: ${Math.round((1 - uint8Array.length / osize) * 1000) / 10}%`;
};

const optimizer = new APNGOptimizer(assembly);
optimizer.checkReady()
    .then(async() => {
        setOriginImage(defaultImage);

        const now = Date.now();
        const optAPNG = await optimizer.optAPNG(defaultImage);
        optTime.innerText = `耗时: ${Date.now() - now}ms`
        setOptImage(optAPNG);
    })