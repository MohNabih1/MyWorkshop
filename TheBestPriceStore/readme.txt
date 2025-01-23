BestPriceStore Database

Item Management System (Part I)
===============================

Part I of the project is a simple web-based Item Management System built using PHP. It allows users to:
- Display items based on specific search criteria.
- Add new items to the system.
- Update existing item details.
- Delete items from the system.

The project is designed for learning purposes and demonstrates basic CRUD operations.

Features
--------
- Display items by ID or name.
- Add new items with unique IDs and details.
- Update item information (name, price, description).
- Delete items by ID.
- Easy-to-use menu interface for navigation.

Requirements
------------
- XAMPP (or any local server running PHP and MySQL)
- PHP 7.4 or higher
- MySQL database

Setup Instructions
------------------
1. Install XAMPP and start the Apache and MySQL servers.
2. Clone or download the project files to the `htdocs` folder of your XAMPP installation.
3. Import the provided SQL file to set up the necessary tables.
4. Open a browser and navigate to `http://localhost/menu.php`.

File Descriptions
-----------------
- `menu.php`: Main menu for navigating between functionalities.
- `displayItem.php`: Allows users to search and display item details.
- `insertItem.php`: Enables users to add new items.
- `updateItem.php`: Facilitates updating existing item details.
- `deleteItem.php`: Allows users to delete items by ID.
- `Service.php`: Contains PHP functions for database operations.
- `Database.php`: Manages database connections.
- `Item.php`: Item Class that includes the item information in the database.


Usage
-----
1. Open the menu page (`http://localhost/menu.php`) in your browser.
2. Select an option (Display, Insert, Update, Delete) by entering the corresponding number.
3. Follow the on-screen prompts to perform the desired action.
4. Use the "Exit" button on each page to return to the menu.

Part II: Vendor Contract View
-----------------------------
In Part II of the project, a view called `VendorContractView` is created to display summarized vendor details. The view includes:
- Vendor ID (`vId`).
- Vendor name (`VendorName`).
- Total number of contracts (`ContractCount`).
- Average price of items supplied by the vendor (`AvgItemPrice`, rounded to the nearest whole number).

Usage
-----
After creating the view, multiple view based queries are to be executed as specified in the project description. 

Acknowledgments
---------------
- [PHP Documentation](https://www.php.net/docs.php)

