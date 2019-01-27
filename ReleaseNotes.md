## Release Notes

### v1.3.2 [dev]

- Fixed bug in flag subtract_fee_from_amount
x Add keccak from -> https://github.com/XKCP/XKCP
x Add ban algorithm
x fix max amount message
x Add LWMA3 diff

### v1.3.1

- Added a new flag subtract_fee_from_amount to the create_transaction method to indicate subtracting fee from receivers.
- Added amounts to the message of the TRANSACTION_TOO_BIG error so that you see how much you can actually send (with desired or zero anonymity).
- Added GetRawBlock method
- Fixed bug when a big transaction persist in mempool
- Fixed bug when txbuilder creates an invalid big tx
- Add timestamp in mempool transaction

### v1.3.0

- Updated for CN Variant 2 - height 265000 ~ 15/dec/2018
  Approximately on the 15th of December (Block #265000) there will be a scheduled network upgrade on the Zelerius network. To be sufficiently prepared, a user, exchange or pool operator, should run daemon v1.3.0 or higher (walletd or zelriusd).

  The scheduled network upgrade introduces one major change. The algorithm changes to CN Variant 2 from CN variant 1 in order to prevent application-specific integrated circuits. See -> https://medium.com/@zelerius/crytonight-variant-2-v8-for-zelerius-network-7ba97489e08e

- Fix minor version from height - larger blocks than upgrade_height_v3 are minor version 2
- Update Check points
- Fix minor bugs

### v1.2.1

- Fixed bug when an invalid transaction may persist in the payment queue.
- Add Math & exception files.
- Use common::integer_cast instead of boost::lexical_cast
- Add payment_id on get_raw_transaction method
- Support Access-Control-Allow-Origin *

### v1.2.0

- A Scheduled Network Upgrade is Planned for October 6.

  Approximately on the 6th of October (Block #64000) there will be a scheduled network upgrade on the Zelerius network. To be sufficiently prepared, a user, or pool operator, should run daemon v1.2.0 or higher (walletd or zelriusd).

  The scheduled network upgrade introduces one major change. The maximum size of a block not resulting into penalty changes to 100000 bytes. Our goal is producing faster blocks on a shorter chain. This change helps to reduce the size of the chain.

- Fix minor bugs.


### v1.1.0

- Fix `get_random_outputs()` function
- Update README
- Rename temporary files & more -> zeleriusd

### v1.0.1

- Rename command arguments.
- Fix "--zeleriusd-authorization=" option.
- Fix Zelerius-Network version.

### v1.0.0

- Project is released.
- Recommended Wallet -> Telegram Wallet https://telegram.me/ZeleriusBot
- Website -> https://zelerius.org
