// 获取API密钥
const apiKey = '6c315b856542e680-Cd0d10b18a7e9edc4-6662efdfbd35ce4d';

// 创建Speedtest对象
const speedtest = new Speedtest(apiKey);

// 开始测试
speedtest.start();

// 监听测试结果
speedtest.on('result', (result) => {
  // 显示测试结果
  document.querySelector('#results').innerHTML = `
    下载速度：${result.download} Mbps
    上传速度：${result.upload} Mbps
    延迟：${result.ping} ms
  `;
});
