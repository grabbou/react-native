let batchedBridge = MessageQueueRe.make();

%raw
{|
Object.defineProperty(global, '__fbBatchedBridge', {
  configurable: true,
  value: batchedBridge,
});

try {
  module.exports = batchedBridge;
} catch (e) {}
|};
