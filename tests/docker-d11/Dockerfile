# Drupal 11 test runner for VAXIA (the port). D7's runner is tests/docker.
FROM php:8.3-apache
RUN apt-get update && apt-get install -y --no-install-recommends \
      libpng-dev libjpeg-dev libwebp-dev libfreetype6-dev libzip-dev libicu-dev unzip git mariadb-client \
  && docker-php-ext-configure gd --with-jpeg --with-webp --with-freetype \
  && docker-php-ext-install gd pdo_mysql zip opcache intl \
  && a2enmod rewrite \
  && rm -rf /var/lib/apt/lists/*
RUN curl -fsSL https://getcomposer.org/download/2.8.12/composer.phar -o /usr/local/bin/composer && chmod +x /usr/local/bin/composer
# PHPUnit as a PHAR: its dependencies are scoped, so they can't clash with the
# Drupal site's vendor directory that the bootstrap loads in-process.
RUN curl -fsSL https://phar.phpunit.de/phpunit-9.6.23.phar -o /usr/local/bin/phpunit && chmod +x /usr/local/bin/phpunit
# The test database container has no TLS (drush passes its own defaults file,
# so it needs the flag too).
RUN mkdir -p /etc/mysql/mariadb.conf.d /etc/drush && printf '[client]\nskip-ssl\n' > /etc/mysql/mariadb.conf.d/99-vaxia.cnf \
  && printf 'command:\n  sql:\n    options:\n      extra: --skip-ssl\n' > /etc/drush/drush.yml
RUN printf 'memory_limit=1G\nmax_execution_time=0\nerror_reporting=E_ALL & ~E_DEPRECATED\nsendmail_path=/bin/true\n' > /usr/local/etc/php/conf.d/vaxia.ini
ENV APACHE_DOCUMENT_ROOT=/var/www/vaxia/web
RUN sed -ri 's!/var/www/html!${APACHE_DOCUMENT_ROOT}!g' /etc/apache2/sites-available/*.conf \
  && sed -ri 's!/var/www/!/var/www/vaxia/web!g' /etc/apache2/apache2.conf \
  && printf '<Directory /var/www/vaxia/web>\n  AllowOverride All\n</Directory>\n' > /etc/apache2/conf-enabled/vaxia.conf
ENV PATH="/var/www/vaxia/vendor/bin:${PATH}"
WORKDIR /var/www/vaxia
