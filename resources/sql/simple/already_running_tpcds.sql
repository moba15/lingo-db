--1
with customer_total_return as
         (select sr_customer_sk as ctr_customer_sk
               ,sr_store_sk as ctr_store_sk
               ,sum(SR_FEE) as ctr_total_return
          from store_returns
             ,date_dim
          where sr_returned_date_sk = d_date_sk
            and d_year =2000
          group by sr_customer_sk
                 ,sr_store_sk)
select  c_customer_id
from customer_total_return ctr1
   ,store
   ,customer
where ctr1.ctr_total_return > (select avg(ctr_total_return)*1.2
                               from customer_total_return ctr2
                               where ctr1.ctr_store_sk = ctr2.ctr_store_sk)
  and s_store_sk = ctr1.ctr_store_sk
  and s_state = 'SD'
  and ctr1.ctr_customer_sk = c_customer_sk
order by c_customer_id
    limit 100;

--2
with wscs as
         (select sold_date_sk
               ,sales_price
          from  (select ws_sold_date_sk sold_date_sk
                      ,ws_ext_sales_price sales_price
                 from web_sales) x
          union all
          select cs_sold_date_sk sold_date_sk
               ,cs_ext_sales_price sales_price
          from catalog_sales),
     wswscs as
         (select d_week_seq,
                 sum(case when (d_day_name='Sunday') then sales_price else null end) sun_sales,
                 sum(case when (d_day_name='Monday') then sales_price else null end) mon_sales,
                 sum(case when (d_day_name='Tuesday') then sales_price else  null end) tue_sales,
                 sum(case when (d_day_name='Wednesday') then sales_price else null end) wed_sales,
                 sum(case when (d_day_name='Thursday') then sales_price else null end) thu_sales,
                 sum(case when (d_day_name='Friday') then sales_price else null end) fri_sales,
                 sum(case when (d_day_name='Saturday') then sales_price else null end) sat_sales
          from wscs
             ,date_dim
          where d_date_sk = sold_date_sk
          group by d_week_seq)
select d_week_seq1
     ,round(sun_sales1/sun_sales2,2)
     ,round(mon_sales1/mon_sales2,2)
     ,round(tue_sales1/tue_sales2,2)
     ,round(wed_sales1/wed_sales2,2)
     ,round(thu_sales1/thu_sales2,2)
     ,round(fri_sales1/fri_sales2,2)
     ,round(sat_sales1/sat_sales2,2)
from
    (select wswscs.d_week_seq d_week_seq1
          ,sun_sales sun_sales1
          ,mon_sales mon_sales1
          ,tue_sales tue_sales1
          ,wed_sales wed_sales1
          ,thu_sales thu_sales1
          ,fri_sales fri_sales1
          ,sat_sales sat_sales1
     from wswscs,date_dim
     where date_dim.d_week_seq = wswscs.d_week_seq and
         d_year = 1998) y,
    (select wswscs.d_week_seq d_week_seq2
          ,sun_sales sun_sales2
          ,mon_sales mon_sales2
          ,tue_sales tue_sales2
          ,wed_sales wed_sales2
          ,thu_sales thu_sales2
          ,fri_sales fri_sales2
          ,sat_sales sat_sales2
     from wswscs
        ,date_dim
     where date_dim.d_week_seq = wswscs.d_week_seq and
         d_year = 1998+1) z
where d_week_seq1=d_week_seq2-53
order by d_week_seq1;

--3
select  dt.d_year
     ,item.i_brand_id brand_id
     ,item.i_brand brand
     ,sum(ss_sales_price) sum_agg
from  date_dim dt
   ,store_sales
   ,item
where dt.d_date_sk = store_sales.ss_sold_date_sk
  and store_sales.ss_item_sk = item.i_item_sk
  and item.i_manufact_id = 816
  and dt.d_moy=11
group by dt.d_year
       ,item.i_brand
       ,item.i_brand_id
order by dt.d_year
       ,sum_agg desc
       ,brand_id
    limit 100;

--4
with year_total as (
    select c_customer_id customer_id
         ,c_first_name customer_first_name
         ,c_last_name customer_last_name
         ,c_preferred_cust_flag customer_preferred_cust_flag
         ,c_birth_country customer_birth_country
         ,c_login customer_login
         ,c_email_address customer_email_address
         ,d_year dyear
         ,sum(((ss_ext_list_price-ss_ext_wholesale_cost-ss_ext_discount_amt)+ss_ext_sales_price)/2) year_total
         ,'s' sale_type
    from customer
       ,store_sales
       ,date_dim
    where c_customer_sk = ss_customer_sk
      and ss_sold_date_sk = d_date_sk
    group by c_customer_id
           ,c_first_name
           ,c_last_name
           ,c_preferred_cust_flag
           ,c_birth_country
           ,c_login
           ,c_email_address
           ,d_year
    union all
    select c_customer_id customer_id
         ,c_first_name customer_first_name
         ,c_last_name customer_last_name
         ,c_preferred_cust_flag customer_preferred_cust_flag
         ,c_birth_country customer_birth_country
         ,c_login customer_login
         ,c_email_address customer_email_address
         ,d_year dyear
         ,sum((((cs_ext_list_price-cs_ext_wholesale_cost-cs_ext_discount_amt)+cs_ext_sales_price)/2) ) year_total
         ,'c' sale_type
    from customer
       ,catalog_sales
       ,date_dim
    where c_customer_sk = cs_bill_customer_sk
      and cs_sold_date_sk = d_date_sk
    group by c_customer_id
           ,c_first_name
           ,c_last_name
           ,c_preferred_cust_flag
           ,c_birth_country
           ,c_login
           ,c_email_address
           ,d_year
    union all
    select c_customer_id customer_id
         ,c_first_name customer_first_name
         ,c_last_name customer_last_name
         ,c_preferred_cust_flag customer_preferred_cust_flag
         ,c_birth_country customer_birth_country
         ,c_login customer_login
         ,c_email_address customer_email_address
         ,d_year dyear
         ,sum((((ws_ext_list_price-ws_ext_wholesale_cost-ws_ext_discount_amt)+ws_ext_sales_price)/2) ) year_total
         ,'w' sale_type
    from customer
       ,web_sales
       ,date_dim
    where c_customer_sk = ws_bill_customer_sk
      and ws_sold_date_sk = d_date_sk
    group by c_customer_id
           ,c_first_name
           ,c_last_name
           ,c_preferred_cust_flag
           ,c_birth_country
           ,c_login
           ,c_email_address
           ,d_year
)
select
    t_s_secyear.customer_id
     ,t_s_secyear.customer_first_name
     ,t_s_secyear.customer_last_name
     ,t_s_secyear.customer_birth_country
from year_total t_s_firstyear
   ,year_total t_s_secyear
   ,year_total t_c_firstyear
   ,year_total t_c_secyear
   ,year_total t_w_firstyear
   ,year_total t_w_secyear
where t_s_secyear.customer_id = t_s_firstyear.customer_id
  and t_s_firstyear.customer_id = t_c_secyear.customer_id
  and t_s_firstyear.customer_id = t_c_firstyear.customer_id
  and t_s_firstyear.customer_id = t_w_firstyear.customer_id
  and t_s_firstyear.customer_id = t_w_secyear.customer_id
  and t_s_firstyear.sale_type = 's'
  and t_c_firstyear.sale_type = 'c'
  and t_w_firstyear.sale_type = 'w'
  and t_s_secyear.sale_type = 's'
  and t_c_secyear.sale_type = 'c'
  and t_w_secyear.sale_type = 'w'
  and t_s_firstyear.dyear =  1999
  and t_s_secyear.dyear = 1999+1
  and t_c_firstyear.dyear =  1999
  and t_c_secyear.dyear =  1999+1
  and t_w_firstyear.dyear = 1999
  and t_w_secyear.dyear = 1999+1
  and t_s_firstyear.year_total > 0
  and t_c_firstyear.year_total > 0
  and t_w_firstyear.year_total > 0
  and case when t_c_firstyear.year_total > 0 then t_c_secyear.year_total / t_c_firstyear.year_total else null end
    > case when t_s_firstyear.year_total > 0 then t_s_secyear.year_total / t_s_firstyear.year_total else null end
  and case when t_c_firstyear.year_total > 0 then t_c_secyear.year_total / t_c_firstyear.year_total else null end
    > case when t_w_firstyear.year_total > 0 then t_w_secyear.year_total / t_w_firstyear.year_total else null end
order by t_s_secyear.customer_id
       ,t_s_secyear.customer_first_name
       ,t_s_secyear.customer_last_name
       ,t_s_secyear.customer_birth_country
    limit 100;
--7
select  i_item_id,
        avg(ss_quantity) agg1,
        avg(ss_list_price) agg2,
        avg(ss_coupon_amt) agg3,
        avg(ss_sales_price) agg4
from store_sales, customer_demographics, date_dim, item, promotion
where ss_sold_date_sk = d_date_sk and
    ss_item_sk = i_item_sk and
    ss_cdemo_sk = cd_demo_sk and
    ss_promo_sk = p_promo_sk and
    cd_gender = 'F' and
    cd_marital_status = 'W' and
    cd_education_status = 'College' and
    (p_channel_email = 'N' or p_channel_event = 'N') and
    d_year = 2001
group by i_item_id
order by i_item_id
    limit 100;
-- 9
select case when (select count(*)
                  from store_sales
                  where ss_quantity between 1 and 20) > 157344
                then (select avg(ss_ext_discount_amt)
                      from store_sales
                      where ss_quantity between 1 and 20)
            else (select avg(ss_net_profit)
                  from store_sales
                  where ss_quantity between 1 and 20) end bucket1 ,
       case when (select count(*)
                  from store_sales
                  where ss_quantity between 21 and 40) > 34708
                then (select avg(ss_ext_discount_amt)
                      from store_sales
                      where ss_quantity between 21 and 40)
            else (select avg(ss_net_profit)
                  from store_sales
                  where ss_quantity between 21 and 40) end bucket2,
       case when (select count(*)
                  from store_sales
                  where ss_quantity between 41 and 60) > 253327
                then (select avg(ss_ext_discount_amt)
                      from store_sales
                      where ss_quantity between 41 and 60)
            else (select avg(ss_net_profit)
                  from store_sales
                  where ss_quantity between 41 and 60) end bucket3,
       case when (select count(*)
                  from store_sales
                  where ss_quantity between 61 and 80) > 272224
                then (select avg(ss_ext_discount_amt)
                      from store_sales
                      where ss_quantity between 61 and 80)
            else (select avg(ss_net_profit)
                  from store_sales
                  where ss_quantity between 61 and 80) end bucket4,
       case when (select count(*)
                  from store_sales
                  where ss_quantity between 81 and 100) > 428113
                then (select avg(ss_ext_discount_amt)
                      from store_sales
                      where ss_quantity between 81 and 100)
            else (select avg(ss_net_profit)
                  from store_sales
                  where ss_quantity between 81 and 100) end bucket5
from reason
where r_reason_sk = 1
;
-- 10
select
    cd_gender,
    cd_marital_status,
    cd_education_status,
    count(*) cnt1,
    cd_purchase_estimate,
    count(*) cnt2,
    cd_credit_rating,
    count(*) cnt3,
    cd_dep_count,
    count(*) cnt4,
    cd_dep_employed_count,
    count(*) cnt5,
    cd_dep_college_count,
    count(*) cnt6
from
    customer c,customer_address ca,customer_demographics
where
    c.c_current_addr_sk = ca.ca_address_sk and
    ca_county in ('Vermilion County','Park County','Dorchester County','Republic County','Hayes County') and
    cd_demo_sk = c.c_current_cdemo_sk and
    exists (select *
            from store_sales,date_dim
            where c.c_customer_sk = ss_customer_sk and
                ss_sold_date_sk = d_date_sk and
                d_year = 2000 and
                d_moy between 4 and 4+3) and
    (exists (select *
             from web_sales,date_dim
             where c.c_customer_sk = ws_bill_customer_sk and
                 ws_sold_date_sk = d_date_sk and
                 d_year = 2000 and
                 d_moy between 4 ANd 4+3) or
     exists (select *
             from catalog_sales,date_dim
             where c.c_customer_sk = cs_ship_customer_sk and
                 cs_sold_date_sk = d_date_sk and
                 d_year = 2000 and
                 d_moy between 4 and 4+3))
group by cd_gender,
         cd_marital_status,
         cd_education_status,
         cd_purchase_estimate,
         cd_credit_rating,
         cd_dep_count,
         cd_dep_employed_count,
         cd_dep_college_count
order by cd_gender,
         cd_marital_status,
         cd_education_status,
         cd_purchase_estimate,
         cd_credit_rating,
         cd_dep_count,
         cd_dep_employed_count,
         cd_dep_college_count
    limit 100;
--11
with year_total as (
    select c_customer_id customer_id
         ,c_first_name customer_first_name
         ,c_last_name customer_last_name
         ,c_preferred_cust_flag customer_preferred_cust_flag
         ,c_birth_country customer_birth_country
         ,c_login customer_login
         ,c_email_address customer_email_address
         ,d_year dyear
         ,sum(ss_ext_list_price-ss_ext_discount_amt) year_total
         ,'s' sale_type
    from customer
       ,store_sales
       ,date_dim
    where c_customer_sk = ss_customer_sk
      and ss_sold_date_sk = d_date_sk
    group by c_customer_id
           ,c_first_name
           ,c_last_name
           ,c_preferred_cust_flag
           ,c_birth_country
           ,c_login
           ,c_email_address
           ,d_year
    union all
    select c_customer_id customer_id
         ,c_first_name customer_first_name
         ,c_last_name customer_last_name
         ,c_preferred_cust_flag customer_preferred_cust_flag
         ,c_birth_country customer_birth_country
         ,c_login customer_login
         ,c_email_address customer_email_address
         ,d_year dyear
         ,sum(ws_ext_list_price-ws_ext_discount_amt) year_total
         ,'w' sale_type
    from customer
       ,web_sales
       ,date_dim
    where c_customer_sk = ws_bill_customer_sk
      and ws_sold_date_sk = d_date_sk
    group by c_customer_id
           ,c_first_name
           ,c_last_name
           ,c_preferred_cust_flag
           ,c_birth_country
           ,c_login
           ,c_email_address
           ,d_year
)
select
    t_s_secyear.customer_id
     ,t_s_secyear.customer_first_name
     ,t_s_secyear.customer_last_name
     ,t_s_secyear.customer_login
from year_total t_s_firstyear
   ,year_total t_s_secyear
   ,year_total t_w_firstyear
   ,year_total t_w_secyear
where t_s_secyear.customer_id = t_s_firstyear.customer_id
  and t_s_firstyear.customer_id = t_w_secyear.customer_id
  and t_s_firstyear.customer_id = t_w_firstyear.customer_id
  and t_s_firstyear.sale_type = 's'
  and t_w_firstyear.sale_type = 'w'
  and t_s_secyear.sale_type = 's'
  and t_w_secyear.sale_type = 'w'
  and t_s_firstyear.dyear = 2001
  and t_s_secyear.dyear = 2001+1
  and t_w_firstyear.dyear = 2001
  and t_w_secyear.dyear = 2001+1
  and t_s_firstyear.year_total > 0
  and t_w_firstyear.year_total > 0
  and case when t_w_firstyear.year_total > 0 then t_w_secyear.year_total / t_w_firstyear.year_total else 0.0 end
    > case when t_s_firstyear.year_total > 0 then t_s_secyear.year_total / t_s_firstyear.year_total else 0.0 end
order by t_s_secyear.customer_id
       ,t_s_secyear.customer_first_name
       ,t_s_secyear.customer_last_name
       ,t_s_secyear.customer_login
    limit 100;

--13
select avg(ss_quantity)
     ,avg(ss_ext_sales_price)
     ,avg(ss_ext_wholesale_cost)
     ,sum(ss_ext_wholesale_cost)
from store_sales
   ,store
   ,customer_demographics
   ,household_demographics
   ,customer_address
   ,date_dim
where s_store_sk = ss_store_sk
  and  ss_sold_date_sk = d_date_sk and d_year = 2001
  and((ss_hdemo_sk=hd_demo_sk
    and cd_demo_sk = ss_cdemo_sk
    and cd_marital_status = 'W'
    and cd_education_status = 'Advanced Degree'
    and ss_sales_price between 100.00 and 150.00
    and hd_dep_count = 3
          )or
      (ss_hdemo_sk=hd_demo_sk
          and cd_demo_sk = ss_cdemo_sk
          and cd_marital_status = 'U'
          and cd_education_status = 'Primary'
          and ss_sales_price between 50.00 and 100.00
          and hd_dep_count = 1
          ) or
      (ss_hdemo_sk=hd_demo_sk
          and cd_demo_sk = ss_cdemo_sk
          and cd_marital_status = 'S'
          and cd_education_status = '4 yr Degree'
          and ss_sales_price between 150.00 and 200.00
          and hd_dep_count = 1
          ))
  and((ss_addr_sk = ca_address_sk
    and ca_country = 'United States'
    and ca_state in ('NE', 'IN', 'NC')
    and ss_net_profit between 100 and 200
          ) or
      (ss_addr_sk = ca_address_sk
          and ca_country = 'United States'
          and ca_state in ('ID', 'GA', 'ME')
          and ss_net_profit between 150 and 300
          ) or
      (ss_addr_sk = ca_address_sk
          and ca_country = 'United States'
          and ca_state in ('TN', 'AL', 'FL')
          and ss_net_profit between 50 and 250
          ))
;
--16
select
    count(distinct cs_order_number) as "order count"
     ,sum(cs_ext_ship_cost) as "total shipping cost"
     ,sum(cs_net_profit) as "total net profit"
from
    catalog_sales cs1
   ,date_dim
   ,customer_address
   ,call_center
where
    d_date between '2002-2-01' and
        (cast('2002-2-01' as date) + interval '60 days')
  and cs1.cs_ship_date_sk = d_date_sk
  and cs1.cs_ship_addr_sk = ca_address_sk
  and ca_state = 'WA'
  and cs1.cs_call_center_sk = cc_call_center_sk
  and cc_county in ('Ziebach County','Walker County','Williamson County','Walker County',
                    'Ziebach County'
    )
  and exists (select *
              from catalog_sales cs2
              where cs1.cs_order_number = cs2.cs_order_number
                and cs1.cs_warehouse_sk <> cs2.cs_warehouse_sk)
  and not exists(select *
                 from catalog_returns cr1
                 where cs1.cs_order_number = cr1.cr_order_number)
order by count(distinct cs_order_number)
    limit 100;

-- 17
select  i_item_id
     ,i_item_desc
     ,s_state
     ,count(ss_quantity) as store_sales_quantitycount
     ,avg(ss_quantity) as store_sales_quantityave
     ,stddev_samp(ss_quantity) as store_sales_quantitystdev
     ,stddev_samp(ss_quantity)/avg(ss_quantity) as store_sales_quantitycov
     ,count(sr_return_quantity) as store_returns_quantitycount
     ,avg(sr_return_quantity) as store_returns_quantityave
     ,stddev_samp(sr_return_quantity) as store_returns_quantitystdev
     ,stddev_samp(sr_return_quantity)/avg(sr_return_quantity) as store_returns_quantitycov
     ,count(cs_quantity) as catalog_sales_quantitycount ,avg(cs_quantity) as catalog_sales_quantityave
     ,stddev_samp(cs_quantity) as catalog_sales_quantitystdev
     ,stddev_samp(cs_quantity)/avg(cs_quantity) as catalog_sales_quantitycov
from store_sales
   ,store_returns
   ,catalog_sales
   ,date_dim d1
   ,date_dim d2
   ,date_dim d3
   ,store
   ,item
where d1.d_quarter_name = '2000Q1'
  and d1.d_date_sk = ss_sold_date_sk
  and i_item_sk = ss_item_sk
  and s_store_sk = ss_store_sk
  and ss_customer_sk = sr_customer_sk
  and ss_item_sk = sr_item_sk
  and ss_ticket_number = sr_ticket_number
  and sr_returned_date_sk = d2.d_date_sk
  and d2.d_quarter_name in ('2000Q1','2000Q2','2000Q3')
  and sr_customer_sk = cs_bill_customer_sk
  and sr_item_sk = cs_item_sk
  and cs_sold_date_sk = d3.d_date_sk
  and d3.d_quarter_name in ('2000Q1','2000Q2','2000Q3')
group by i_item_id
       ,i_item_desc
       ,s_state
order by i_item_id
       ,i_item_desc
       ,s_state
    limit 100;
-- 21
select  *
from(select w_warehouse_name
          ,i_item_id
          ,sum(case when (cast(d_date as date) < cast ('2002-05-26' as date))
                        then inv_quantity_on_hand
                    else 0 end) as inv_before
          ,sum(case when (cast(d_date as date) >= cast ('2002-05-26' as date))
                        then inv_quantity_on_hand
                    else 0 end) as inv_after
     from inventory
        ,warehouse
        ,item
        ,date_dim
     where i_current_price between 0.99 and 1.49
       and i_item_sk          = inv_item_sk
       and inv_warehouse_sk   = w_warehouse_sk
       and inv_date_sk    = d_date_sk
       and d_date between (cast ('2002-05-26' as date) - interval '30 days')
         and (cast ('2002-05-26' as date) + interval '30 days')
     group by w_warehouse_name, i_item_id) x
where (case when inv_before > 0
                then inv_after / inv_before
            else null
    end) between 2.0/3.0 and 3.0/2.0
order by w_warehouse_name
       ,i_item_id
    limit 100;
-- 58
with ss_items as
         (select i_item_id item_id
               ,sum(ss_ext_sales_price) ss_item_rev
          from store_sales
             ,item
             ,date_dim
          where ss_item_sk = i_item_sk
            and d_date in (select d_date
                           from date_dim
                           where d_week_seq = (select d_week_seq
                                               from date_dim
                                               where d_date = '1998-06-23'))
            and ss_sold_date_sk   = d_date_sk
          group by i_item_id),
     cs_items as
         (select i_item_id item_id
               ,sum(cs_ext_sales_price) cs_item_rev
          from catalog_sales
             ,item
             ,date_dim
          where cs_item_sk = i_item_sk
            and  d_date in (select d_date
                            from date_dim
                            where d_week_seq = (select d_week_seq
                                                from date_dim
                                                where d_date = '1998-06-23'))
            and  cs_sold_date_sk = d_date_sk
          group by i_item_id),
     ws_items as
         (select i_item_id item_id
               ,sum(ws_ext_sales_price) ws_item_rev
          from web_sales
             ,item
             ,date_dim
          where ws_item_sk = i_item_sk
            and  d_date in (select d_date
                            from date_dim
                            where d_week_seq =(select d_week_seq
                                               from date_dim
                                               where d_date = '1998-06-23'))
            and ws_sold_date_sk   = d_date_sk
          group by i_item_id)
select  ss_items.item_id
     ,ss_item_rev
     ,ss_item_rev/((ss_item_rev+cs_item_rev+ws_item_rev)/3) * 100 ss_dev
     ,cs_item_rev
     ,cs_item_rev/((ss_item_rev+cs_item_rev+ws_item_rev)/3) * 100 cs_dev
     ,ws_item_rev
     ,ws_item_rev/((ss_item_rev+cs_item_rev+ws_item_rev)/3) * 100 ws_dev
     ,(ss_item_rev+cs_item_rev+ws_item_rev)/3 average
from ss_items,cs_items,ws_items
where ss_items.item_id=cs_items.item_id
  and ss_items.item_id=ws_items.item_id
  and ss_item_rev between 0.9 * cs_item_rev and 1.1 * cs_item_rev
  and ss_item_rev between 0.9 * ws_item_rev and 1.1 * ws_item_rev
  and cs_item_rev between 0.9 * ss_item_rev and 1.1 * ss_item_rev
  and cs_item_rev between 0.9 * ws_item_rev and 1.1 * ws_item_rev
  and ws_item_rev between 0.9 * ss_item_rev and 1.1 * ss_item_rev
  and ws_item_rev between 0.9 * cs_item_rev and 1.1 * cs_item_rev
order by item_id
       ,ss_item_rev
    limit 100;

-- 25
select
    i_item_id
     ,i_item_desc
     ,s_store_id
     ,s_store_name
     ,stddev_samp(ss_net_profit) as store_sales_profit
     ,stddev_samp(sr_net_loss) as store_returns_loss
     ,stddev_samp(cs_net_profit) as catalog_sales_profit
from
    store_sales
   ,store_returns
   ,catalog_sales
   ,date_dim d1
   ,date_dim d2
   ,date_dim d3
   ,store
   ,item
where
    d1.d_moy = 4
  and d1.d_year = 2002
  and d1.d_date_sk = ss_sold_date_sk
  and i_item_sk = ss_item_sk
  and s_store_sk = ss_store_sk
  and ss_customer_sk = sr_customer_sk
  and ss_item_sk = sr_item_sk
  and ss_ticket_number = sr_ticket_number
  and sr_returned_date_sk = d2.d_date_sk
  and d2.d_moy               between 4 and  10
  and d2.d_year              = 2002
  and sr_customer_sk = cs_bill_customer_sk
  and sr_item_sk = cs_item_sk
  and cs_sold_date_sk = d3.d_date_sk
  and d3.d_moy               between 4 and  10
  and d3.d_year              = 2002
group by
    i_item_id
       ,i_item_desc
       ,s_store_id
       ,s_store_name
order by
    i_item_id
       ,i_item_desc
       ,s_store_id
       ,s_store_name
    limit 100;


-- 26
select  i_item_id,
        avg(cs_quantity) agg1,
        avg(cs_list_price) agg2,
        avg(cs_coupon_amt) agg3,
        avg(cs_sales_price) agg4
from catalog_sales, customer_demographics, date_dim, item, promotion
where cs_sold_date_sk = d_date_sk and
    cs_item_sk = i_item_sk and
    cs_bill_cdemo_sk = cd_demo_sk and
    cs_promo_sk = p_promo_sk and
    cd_gender = 'M' and
    cd_marital_status = 'D' and
    cd_education_status = 'Secondary' and
    (p_channel_email = 'N' or p_channel_event = 'N') and
    d_year = 2001
group by i_item_id
order by i_item_id
    limit 100;
--30
with customer_total_return as
         (select wr_returning_customer_sk as ctr_customer_sk
                  ,ca_state as ctr_state,
                 sum(wr_return_amt) as ctr_total_return
          from web_returns
             ,date_dim
             ,customer_address
          where wr_returned_date_sk = d_date_sk
            and d_year =2001
            and wr_returning_addr_sk = ca_address_sk
          group by wr_returning_customer_sk
                 ,ca_state)
select  c_customer_id,c_salutation,c_first_name,c_last_name,c_preferred_cust_flag
     ,c_birth_day,c_birth_month,c_birth_year,c_birth_country,c_login,c_email_address
     ,c_last_review_date_sk,ctr_total_return
from customer_total_return ctr1
   ,customer_address
   ,customer
where ctr1.ctr_total_return > (select avg(ctr_total_return)*1.2
                               from customer_total_return ctr2
                               where ctr1.ctr_state = ctr2.ctr_state)
  and ca_address_sk = c_current_addr_sk
  and ca_state = 'GA'
  and ctr1.ctr_customer_sk = c_customer_sk
order by c_customer_id,c_salutation,c_first_name,c_last_name,c_preferred_cust_flag
       ,c_birth_day,c_birth_month,c_birth_year,c_birth_country,c_login,c_email_address
       ,c_last_review_date_sk,ctr_total_return
    limit 100;

--31
with ss as
         (select ca_county,d_qoy, d_year,sum(ss_ext_sales_price) as store_sales
          from store_sales,date_dim,customer_address
          where ss_sold_date_sk = d_date_sk
            and ss_addr_sk=ca_address_sk
          group by ca_county,d_qoy, d_year),
     ws as
         (select ca_county,d_qoy, d_year,sum(ws_ext_sales_price) as web_sales
          from web_sales,date_dim,customer_address
          where ws_sold_date_sk = d_date_sk
            and ws_bill_addr_sk=ca_address_sk
          group by ca_county,d_qoy, d_year)
select
    ss1.ca_county
     ,ss1.d_year
     ,ws2.web_sales/ws1.web_sales web_q1_q2_increase
     ,ss2.store_sales/ss1.store_sales store_q1_q2_increase
     ,ws3.web_sales/ws2.web_sales web_q2_q3_increase
     ,ss3.store_sales/ss2.store_sales store_q2_q3_increase
from
    ss ss1
   ,ss ss2
   ,ss ss3
   ,ws ws1
   ,ws ws2
   ,ws ws3
where
    ss1.d_qoy = 1
  and ss1.d_year = 2000
  and ss1.ca_county = ss2.ca_county
  and ss2.d_qoy = 2
  and ss2.d_year = 2000
  and ss2.ca_county = ss3.ca_county
  and ss3.d_qoy = 3
  and ss3.d_year = 2000
  and ss1.ca_county = ws1.ca_county
  and ws1.d_qoy = 1
  and ws1.d_year = 2000
  and ws1.ca_county = ws2.ca_county
  and ws2.d_qoy = 2
  and ws2.d_year = 2000
  and ws1.ca_county = ws3.ca_county
  and ws3.d_qoy = 3
  and ws3.d_year =2000
  and case when ws1.web_sales > 0 then ws2.web_sales/ws1.web_sales else null end
    > case when ss1.store_sales > 0 then ss2.store_sales/ss1.store_sales else null end
  and case when ws2.web_sales > 0 then ws3.web_sales/ws2.web_sales else null end
    > case when ss2.store_sales > 0 then ss3.store_sales/ss2.store_sales else null end
order by store_q2_q3_increase;
--3
with ss as (
    select
        i_manufact_id,sum(ss_ext_sales_price) total_sales
    from
        store_sales,
        date_dim,
        customer_address,
        item
    where
        i_manufact_id in (select
                              i_manufact_id
                          from
                              item
                          where i_category in ('Jewelry'))
      and     ss_item_sk              = i_item_sk
      and     ss_sold_date_sk         = d_date_sk
      and     d_year                  = 2000
      and     d_moy                   = 6
      and     ss_addr_sk              = ca_address_sk
      and     ca_gmt_offset           = -6
    group by i_manufact_id),
     cs as (
         select
             i_manufact_id,sum(cs_ext_sales_price) total_sales
         from
             catalog_sales,
             date_dim,
             customer_address,
             item
         where
             i_manufact_id               in (select
                                                 i_manufact_id
                                             from
                                                 item
                                             where i_category in ('Jewelry'))
           and     cs_item_sk              = i_item_sk
           and     cs_sold_date_sk         = d_date_sk
           and     d_year                  = 2000
           and     d_moy                   = 6
           and     cs_bill_addr_sk         = ca_address_sk
           and     ca_gmt_offset           = -6
         group by i_manufact_id),
     ws as (
         select
             i_manufact_id,sum(ws_ext_sales_price) total_sales
         from
             web_sales,
             date_dim,
             customer_address,
             item
         where
             i_manufact_id               in (select
                                                 i_manufact_id
                                             from
                                                 item
                                             where i_category in ('Jewelry'))
           and     ws_item_sk              = i_item_sk
           and     ws_sold_date_sk         = d_date_sk
           and     d_year                  = 2000
           and     d_moy                   = 6
           and     ws_bill_addr_sk         = ca_address_sk
           and     ca_gmt_offset           = -6
         group by i_manufact_id)
select  i_manufact_id ,sum(total_sales) total_sales
from  (select * from ss
       union all
       select * from cs
       union all
       select * from ws) tmp1
group by i_manufact_id
order by total_sales
    limit 100;

--35
select
    ca_state,
    cd_gender,
    cd_marital_status,
    cd_dep_count,
    count(*) cnt1,
    sum(cd_dep_count),
    min(cd_dep_count),
    stddev_samp(cd_dep_count),
    cd_dep_employed_count,
    count(*) cnt2,
    sum(cd_dep_employed_count),
    min(cd_dep_employed_count),
    stddev_samp(cd_dep_employed_count),
    cd_dep_college_count,
    count(*) cnt3,
    sum(cd_dep_college_count),
    min(cd_dep_college_count),
    stddev_samp(cd_dep_college_count)
from
    customer c,customer_address ca,customer_demographics
where
    c.c_current_addr_sk = ca.ca_address_sk and
    cd_demo_sk = c.c_current_cdemo_sk and
    exists (select *
            from store_sales,date_dim
            where c.c_customer_sk = ss_customer_sk and
                ss_sold_date_sk = d_date_sk and
                d_year = 1999 and
                d_qoy < 4) and
    (exists (select *
             from web_sales,date_dim
             where c.c_customer_sk = ws_bill_customer_sk and
                 ws_sold_date_sk = d_date_sk and
                 d_year = 1999 and
                 d_qoy < 4) or
     exists (select *
             from catalog_sales,date_dim
             where c.c_customer_sk = cs_ship_customer_sk and
                 cs_sold_date_sk = d_date_sk and
                 d_year = 1999 and
                 d_qoy < 4))
group by ca_state,
         cd_gender,
         cd_marital_status,
         cd_dep_count,
         cd_dep_employed_count,
         cd_dep_college_count
order by ca_state,
         cd_gender,
         cd_marital_status,
         cd_dep_count,
         cd_dep_employed_count,
         cd_dep_college_count
    limit 100;
-- 61
select  promotions,total,cast(promotions as decimal(15,4))/cast(total as decimal(15,4))*100
from
    (select sum(ss_ext_sales_price) promotions
     from  store_sales
        ,store
        ,promotion
        ,date_dim
        ,customer
        ,customer_address
        ,item
     where ss_sold_date_sk = d_date_sk
       and   ss_store_sk = s_store_sk
       and   ss_promo_sk = p_promo_sk
       and   ss_customer_sk= c_customer_sk
       and   ca_address_sk = c_current_addr_sk
       and   ss_item_sk = i_item_sk
       and   ca_gmt_offset = -7
       and   i_category = 'Books'
       and   (p_channel_dmail = 'Y' or p_channel_email = 'Y' or p_channel_tv = 'Y')
       and   s_gmt_offset = -7
       and   d_year = 2001
       and   d_moy  = 12) promotional_sales,
    (select sum(ss_ext_sales_price) total
     from  store_sales
        ,store
        ,date_dim
        ,customer
        ,customer_address
        ,item
     where ss_sold_date_sk = d_date_sk
       and   ss_store_sk = s_store_sk
       and   ss_customer_sk= c_customer_sk
       and   ca_address_sk = c_current_addr_sk
       and   ss_item_sk = i_item_sk
       and   ca_gmt_offset = -7
       and   i_category = 'Books'
       and   s_gmt_offset = -7
       and   d_year = 2001
       and   d_moy  = 12) all_sales
order by promotions, total
    limit 100;
-- 32
select  sum(cs_ext_discount_amt)  as "excess discount amount"
from
    catalog_sales
   ,item
   ,date_dim
where
    i_manufact_id = 994
  and i_item_sk = cs_item_sk
  and d_date between '2001-03-14' and
    (cast('2001-03-14' as date) + interval '90 days')
  and d_date_sk = cs_sold_date_sk
  and cs_ext_discount_amt
    > (
          select
              1.3 * avg(cs_ext_discount_amt)
          from
              catalog_sales
             ,date_dim
          where
              cs_item_sk = i_item_sk
            and d_date between '2001-03-14' and
              (cast('2001-03-14' as date) + interval '90 days')
            and d_date_sk = cs_sold_date_sk
      )
    limit 100;
-- 28
select  *
from (select avg(ss_list_price) B1_LP
           ,count(ss_list_price) B1_CNT
           ,count(distinct ss_list_price) B1_CNTD
      from store_sales
      where ss_quantity between 0 and 5
        and (ss_list_price between 32 and 32+10
          or ss_coupon_amt between 4429 and 4429+1000
          or ss_wholesale_cost between 0 and 0+20)) B1,
     (select avg(ss_list_price) B2_LP
           ,count(ss_list_price) B2_CNT
           ,count(distinct ss_list_price) B2_CNTD
      from store_sales
      where ss_quantity between 6 and 10
        and (ss_list_price between 170 and 170+10
          or ss_coupon_amt between 4727 and 4727+1000
          or ss_wholesale_cost between 8 and 8+20)) B2,
     (select avg(ss_list_price) B3_LP
           ,count(ss_list_price) B3_CNT
           ,count(distinct ss_list_price) B3_CNTD
      from store_sales
      where ss_quantity between 11 and 15
        and (ss_list_price between 124 and 124+10
          or ss_coupon_amt between 9657 and 9657+1000
          or ss_wholesale_cost between 43 and 43+20)) B3,
     (select avg(ss_list_price) B4_LP
           ,count(ss_list_price) B4_CNT
           ,count(distinct ss_list_price) B4_CNTD
      from store_sales
      where ss_quantity between 16 and 20
        and (ss_list_price between 104 and 104+10
          or ss_coupon_amt between 17161 and 17161+1000
          or ss_wholesale_cost between 34 and 34+20)) B4,
     (select avg(ss_list_price) B5_LP
           ,count(ss_list_price) B5_CNT
           ,count(distinct ss_list_price) B5_CNTD
      from store_sales
      where ss_quantity between 21 and 25
        and (ss_list_price between 73 and 73+10
          or ss_coupon_amt between 17064 and 17064+1000
          or ss_wholesale_cost between 56 and 56+20)) B5,
     (select avg(ss_list_price) B6_LP
           ,count(ss_list_price) B6_CNT
           ,count(distinct ss_list_price) B6_CNTD
      from store_sales
      where ss_quantity between 26 and 30
        and (ss_list_price between 65 and 65+10
          or ss_coupon_amt between 10899 and 10899+1000
          or ss_wholesale_cost between 54 and 54+20)) B6
    limit 100;
-- 15
select  ca_zip
     ,sum(cs_sales_price)
from catalog_sales
   ,customer
   ,customer_address
   ,date_dim
where cs_bill_customer_sk = c_customer_sk
  and c_current_addr_sk = ca_address_sk
  and ( substr(ca_zip,1,5) in ('85669', '86197','88274','83405','86475',
                               '85392', '85460', '80348', '81792')
    or ca_state in ('CA','WA','GA')
    or cs_sales_price > 500)
  and cs_sold_date_sk = d_date_sk
  and d_qoy = 1 and d_year = 2000
group by ca_zip
order by ca_zip
    limit 100;
-- 24a
with ssales as
         (select c_last_name
               ,c_first_name
               ,s_store_name
               ,ca_state
               ,s_state
               ,i_color
               ,i_current_price
               ,i_manager_id
               ,i_units
               ,i_size
               ,sum(ss_sales_price) netpaid
          from store_sales
             ,store_returns
             ,store
             ,item
             ,customer
             ,customer_address
          where ss_ticket_number = sr_ticket_number
            and ss_item_sk = sr_item_sk
            and ss_customer_sk = c_customer_sk
            and ss_item_sk = i_item_sk
            and ss_store_sk = s_store_sk
            and c_current_addr_sk = ca_address_sk
            and c_birth_country <> upper(ca_country)
            and s_zip = ca_zip
            and s_market_id=9
          group by c_last_name
                 ,c_first_name
                 ,s_store_name
                 ,ca_state
                 ,s_state
                 ,i_color
                 ,i_current_price
                 ,i_manager_id
                 ,i_units
                 ,i_size)
select c_last_name
     ,c_first_name
     ,s_store_name
     ,sum(netpaid) paid
from ssales
where i_color = 'chocolate'
group by c_last_name
       ,c_first_name
       ,s_store_name
having sum(netpaid) > (select 0.05*avg(netpaid)
                       from ssales)
order by c_last_name
       ,c_first_name
       ,s_store_name
;
-- 24b
with ssales as
         (select c_last_name
               ,c_first_name
               ,s_store_name
               ,ca_state
               ,s_state
               ,i_color
               ,i_current_price
               ,i_manager_id
               ,i_units
               ,i_size
               ,sum(ss_sales_price) netpaid
          from store_sales
             ,store_returns
             ,store
             ,item
             ,customer
             ,customer_address
          where ss_ticket_number = sr_ticket_number
            and ss_item_sk = sr_item_sk
            and ss_customer_sk = c_customer_sk
            and ss_item_sk = i_item_sk
            and ss_store_sk = s_store_sk
            and c_current_addr_sk = ca_address_sk
            and c_birth_country <> upper(ca_country)
            and s_zip = ca_zip
            and s_market_id = 9
          group by c_last_name
                 ,c_first_name
                 ,s_store_name
                 ,ca_state
                 ,s_state
                 ,i_color
                 ,i_current_price
                 ,i_manager_id
                 ,i_units
                 ,i_size)
select c_last_name
     ,c_first_name
     ,s_store_name
     ,sum(netpaid) paid
from ssales
where i_color = 'lawn'
group by c_last_name
       ,c_first_name
       ,s_store_name
having sum(netpaid) > (select 0.05*avg(netpaid)
                       from ssales)
order by c_last_name
       ,c_first_name
       ,s_store_name
;
--6
select  a.ca_state state, count(*) cnt
from customer_address a
   ,customer c
   ,store_sales s
   ,date_dim d
   ,item i
where       a.ca_address_sk = c.c_current_addr_sk
  and c.c_customer_sk = s.ss_customer_sk
  and s.ss_sold_date_sk = d.d_date_sk
  and s.ss_item_sk = i.i_item_sk
  and d.d_month_seq =
      (select distinct (d_month_seq)
       from date_dim
       where d_year = 2002
         and d_moy = 3 )
  and i.i_current_price > 1.2 *
                          (select avg(j.i_current_price)
                           from item j
                           where j.i_category = i.i_category)
group by a.ca_state
having count(*) >= 10
order by cnt, a.ca_state
    limit 100;
-- 42
select  dt.d_year
     ,item.i_category_id
     ,item.i_category
     ,sum(ss_ext_sales_price)
from 	date_dim dt
   ,store_sales
   ,item
where dt.d_date_sk = store_sales.ss_sold_date_sk
  and store_sales.ss_item_sk = item.i_item_sk
  and item.i_manager_id = 1
  and dt.d_moy=12
  and dt.d_year=1999
group by 	dt.d_year
       ,item.i_category_id
       ,item.i_category
order by       sum(ss_ext_sales_price) desc,dt.d_year
       ,item.i_category_id
       ,item.i_category
    limit 100 ;
-- 45
select  ca_zip, ca_city, sum(ws_sales_price)
from web_sales, customer, customer_address, date_dim, item
where ws_bill_customer_sk = c_customer_sk
  and c_current_addr_sk = ca_address_sk
  and ws_item_sk = i_item_sk
  and ( substr(ca_zip,1,5) in ('85669', '86197','88274','83405','86475', '85392', '85460', '80348', '81792')
    or
        i_item_id in (select i_item_id
                      from item
                      where i_item_sk in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29)
        )
    )
  and ws_sold_date_sk = d_date_sk
  and d_qoy = 2 and d_year = 2001
group by ca_zip, ca_city
order by ca_zip, ca_city
    limit 100;
-- 8
select  s_store_name
     ,sum(ss_net_profit)
from store_sales
   ,date_dim
   ,store,
    (select ca_zip
     from (
              SELECT substr(ca_zip,1,5) ca_zip
              FROM customer_address
              WHERE substr(ca_zip,1,5) IN (
                                           '47602','16704','35863','28577','83910','36201',
                                           '58412','48162','28055','41419','80332',
                                           '38607','77817','24891','16226','18410',
                                           '21231','59345','13918','51089','20317',
                                           '17167','54585','67881','78366','47770',
                                           '18360','51717','73108','14440','21800',
                                           '89338','45859','65501','34948','25973',
                                           '73219','25333','17291','10374','18829',
                                           '60736','82620','41351','52094','19326',
                                           '25214','54207','40936','21814','79077',
                                           '25178','75742','77454','30621','89193',
                                           '27369','41232','48567','83041','71948',
                                           '37119','68341','14073','16891','62878',
                                           '49130','19833','24286','27700','40979',
                                           '50412','81504','94835','84844','71954',
                                           '39503','57649','18434','24987','12350',
                                           '86379','27413','44529','98569','16515',
                                           '27287','24255','21094','16005','56436',
                                           '91110','68293','56455','54558','10298',
                                           '83647','32754','27052','51766','19444',
                                           '13869','45645','94791','57631','20712',
                                           '37788','41807','46507','21727','71836',
                                           '81070','50632','88086','63991','20244',
                                           '31655','51782','29818','63792','68605',
                                           '94898','36430','57025','20601','82080',
                                           '33869','22728','35834','29086','92645',
                                           '98584','98072','11652','78093','57553',
                                           '43830','71144','53565','18700','90209',
                                           '71256','38353','54364','28571','96560',
                                           '57839','56355','50679','45266','84680',
                                           '34306','34972','48530','30106','15371',
                                           '92380','84247','92292','68852','13338',
                                           '34594','82602','70073','98069','85066',
                                           '47289','11686','98862','26217','47529',
                                           '63294','51793','35926','24227','14196',
                                           '24594','32489','99060','49472','43432',
                                           '49211','14312','88137','47369','56877',
                                           '20534','81755','15794','12318','21060',
                                           '73134','41255','63073','81003','73873',
                                           '66057','51184','51195','45676','92696',
                                           '70450','90669','98338','25264','38919',
                                           '59226','58581','60298','17895','19489',
                                           '52301','80846','95464','68770','51634',
                                           '19988','18367','18421','11618','67975',
                                           '25494','41352','95430','15734','62585',
                                           '97173','33773','10425','75675','53535',
                                           '17879','41967','12197','67998','79658',
                                           '59130','72592','14851','43933','68101',
                                           '50636','25717','71286','24660','58058',
                                           '72991','95042','15543','33122','69280',
                                           '11912','59386','27642','65177','17672',
                                           '33467','64592','36335','54010','18767',
                                           '63193','42361','49254','33113','33159',
                                           '36479','59080','11855','81963','31016',
                                           '49140','29392','41836','32958','53163',
                                           '13844','73146','23952','65148','93498',
                                           '14530','46131','58454','13376','13378',
                                           '83986','12320','17193','59852','46081',
                                           '98533','52389','13086','68843','31013',
                                           '13261','60560','13443','45533','83583',
                                           '11489','58218','19753','22911','25115',
                                           '86709','27156','32669','13123','51933',
                                           '39214','41331','66943','14155','69998',
                                           '49101','70070','35076','14242','73021',
                                           '59494','15782','29752','37914','74686',
                                           '83086','34473','15751','81084','49230',
                                           '91894','60624','17819','28810','63180',
                                           '56224','39459','55233','75752','43639',
                                           '55349','86057','62361','50788','31830',
                                           '58062','18218','85761','60083','45484',
                                           '21204','90229','70041','41162','35390',
                                           '16364','39500','68908','26689','52868',
                                           '81335','40146','11340','61527','61794',
                                           '71997','30415','59004','29450','58117',
                                           '69952','33562','83833','27385','61860',
                                           '96435','48333','23065','32961','84919',
                                           '61997','99132','22815','56600','68730',
                                           '48017','95694','32919','88217','27116',
                                           '28239','58032','18884','16791','21343',
                                           '97462','18569','75660','15475')
              intersect
              select ca_zip
              from (SELECT substr(ca_zip,1,5) ca_zip,count(*) cnt
                    FROM customer_address, customer
                    WHERE ca_address_sk = c_current_addr_sk and
                        c_preferred_cust_flag='Y'
                    group by ca_zip
                    having count(*) > 10)A1)A2) V1
where ss_store_sk = s_store_sk
  and ss_sold_date_sk = d_date_sk
  and d_qoy = 2 and d_year = 1998
  and (substr(s_zip,1,2) = substr(V1.ca_zip,1,2))
group by s_store_name
order by s_store_name
    limit 100;
--14 b
with  cross_items as
          (select i_item_sk ss_item_sk
           from item,
                (select iss.i_brand_id brand_id
                      ,iss.i_class_id class_id
                      ,iss.i_category_id category_id
                 from store_sales
                    ,item iss
                    ,date_dim d1
                 where ss_item_sk = iss.i_item_sk
                   and ss_sold_date_sk = d1.d_date_sk
                   and d1.d_year between 1998 AND 1998 + 2
                 intersect
                 select ics.i_brand_id
                      ,ics.i_class_id
                      ,ics.i_category_id
                 from catalog_sales
                    ,item ics
                    ,date_dim d2
                 where cs_item_sk = ics.i_item_sk
                   and cs_sold_date_sk = d2.d_date_sk
                   and d2.d_year between 1998 AND 1998 + 2
                 intersect
                 select iws.i_brand_id
                      ,iws.i_class_id
                      ,iws.i_category_id
                 from web_sales
                    ,item iws
                    ,date_dim d3
                 where ws_item_sk = iws.i_item_sk
                   and ws_sold_date_sk = d3.d_date_sk
                   and d3.d_year between 1998 AND 1998 + 2) x
           where i_brand_id = brand_id
             and i_class_id = class_id
             and i_category_id = category_id
          ),
      avg_sales as
          (select avg(quantity*list_price) average_sales
           from (select ss_quantity quantity
                      ,ss_list_price list_price
                 from store_sales
                    ,date_dim
                 where ss_sold_date_sk = d_date_sk
                   and d_year between 1998 and 1998 + 2
                 union all
                 select cs_quantity quantity
                      ,cs_list_price list_price
                 from catalog_sales
                    ,date_dim
                 where cs_sold_date_sk = d_date_sk
                   and d_year between 1998 and 1998 + 2
                 union all
                 select ws_quantity quantity
                      ,ws_list_price list_price
                 from web_sales
                    ,date_dim
                 where ws_sold_date_sk = d_date_sk
                   and d_year between 1998 and 1998 + 2) x)
select  * from
              (select 'store' channel, i_brand_id,i_class_id,i_category_id
                    ,sum(ss_quantity*ss_list_price) sales, count(*) number_sales
               from store_sales
                  ,item
                  ,date_dim
               where ss_item_sk in (select ss_item_sk from cross_items)
                 and ss_item_sk = i_item_sk
                 and ss_sold_date_sk = d_date_sk
                 and d_week_seq = (select d_week_seq
                                   from date_dim
                                   where d_year = 1998 + 1
                                     and d_moy = 12
                                     and d_dom = 20)
               group by i_brand_id,i_class_id,i_category_id
               having sum(ss_quantity*ss_list_price) > (select average_sales from avg_sales)) this_year,
              (select 'store' channel, i_brand_id,i_class_id
                    ,i_category_id, sum(ss_quantity*ss_list_price) sales, count(*) number_sales
               from store_sales
                  ,item
                  ,date_dim
               where ss_item_sk in (select ss_item_sk from cross_items)
                 and ss_item_sk = i_item_sk
                 and ss_sold_date_sk = d_date_sk
                 and d_week_seq = (select d_week_seq
                                   from date_dim
                                   where d_year = 1998
                                     and d_moy = 12
                                     and d_dom = 20)
               group by i_brand_id,i_class_id,i_category_id
               having sum(ss_quantity*ss_list_price) > (select average_sales from avg_sales)) last_year
where this_year.i_brand_id= last_year.i_brand_id
  and this_year.i_class_id = last_year.i_class_id
  and this_year.i_category_id = last_year.i_category_id
order by this_year.channel, this_year.i_brand_id, this_year.i_class_id, this_year.i_category_id
    limit 100;
--29
select
    i_item_id
     ,i_item_desc
     ,s_store_id
     ,s_store_name
     ,stddev_samp(ss_quantity)        as store_sales_quantity
     ,stddev_samp(sr_return_quantity) as store_returns_quantity
     ,stddev_samp(cs_quantity)        as catalog_sales_quantity
from
    store_sales
   ,store_returns
   ,catalog_sales
   ,date_dim             d1
   ,date_dim             d2
   ,date_dim             d3
   ,store
   ,item
where
    d1.d_moy               = 4
  and d1.d_year              = 2000
  and d1.d_date_sk           = ss_sold_date_sk
  and i_item_sk              = ss_item_sk
  and s_store_sk             = ss_store_sk
  and ss_customer_sk         = sr_customer_sk
  and ss_item_sk             = sr_item_sk
  and ss_ticket_number       = sr_ticket_number
  and sr_returned_date_sk    = d2.d_date_sk
  and d2.d_moy               between 4 and  4 + 3
  and d2.d_year              = 2000
  and sr_customer_sk         = cs_bill_customer_sk
  and sr_item_sk             = cs_item_sk
  and cs_sold_date_sk        = d3.d_date_sk
  and d3.d_year              in (2000,2000+1,2000+2)
group by
    i_item_id
       ,i_item_desc
       ,s_store_id
       ,s_store_name
order by
    i_item_id
       ,i_item_desc
       ,s_store_id
       ,s_store_name
    limit 100;
-- 34
select c_last_name
     ,c_first_name
     ,c_salutation
     ,c_preferred_cust_flag
     ,ss_ticket_number
     ,cnt from
              (select ss_ticket_number
                    ,ss_customer_sk
                    ,count(*) cnt
               from store_sales,date_dim,store,household_demographics
               where store_sales.ss_sold_date_sk = date_dim.d_date_sk
                 and store_sales.ss_store_sk = store.s_store_sk
                 and store_sales.ss_hdemo_sk = household_demographics.hd_demo_sk
                 and (date_dim.d_dom between 1 and 3 or date_dim.d_dom between 25 and 28)
                 and (household_demographics.hd_buy_potential = '>10000' or
                      household_demographics.hd_buy_potential = '0-500')
                 and household_demographics.hd_vehicle_count > 0
                 and (case when household_demographics.hd_vehicle_count > 0
                               then household_demographics.hd_dep_count/ household_demographics.hd_vehicle_count
                           else null
                   end)  > 1.2
                 and date_dim.d_year in (2000,2000+1,2000+2)
                 and store.s_county in ('Ziebach County','Williamson County','Walker County','Ziebach County',
                                        'Ziebach County','Ziebach County','Ziebach County','Ziebach County')
               group by ss_ticket_number,ss_customer_sk) dn,customer
where ss_customer_sk = c_customer_sk
  and cnt between 15 and 20
order by c_last_name,c_first_name,c_salutation,c_preferred_cust_flag desc, ss_ticket_number;
-- 38
select  count(*) from (
                          select distinct c_last_name, c_first_name, d_date
                          from store_sales, date_dim, customer
                          where store_sales.ss_sold_date_sk = date_dim.d_date_sk
                            and store_sales.ss_customer_sk = customer.c_customer_sk
                            and d_month_seq between 1179 and 1179 + 11
                          intersect
                          select distinct c_last_name, c_first_name, d_date
                          from catalog_sales, date_dim, customer
                          where catalog_sales.cs_sold_date_sk = date_dim.d_date_sk
                            and catalog_sales.cs_bill_customer_sk = customer.c_customer_sk
                            and d_month_seq between 1179 and 1179 + 11
                          intersect
                          select distinct c_last_name, c_first_name, d_date
                          from web_sales, date_dim, customer
                          where web_sales.ws_sold_date_sk = date_dim.d_date_sk
                            and web_sales.ws_bill_customer_sk = customer.c_customer_sk
                            and d_month_seq between 1179 and 1179 + 11
                      ) hot_cust
    limit 100;
-- 46
select  c_last_name
     ,c_first_name
     ,ca_city
     ,bought_city
     ,ss_ticket_number
     ,amt,profit
from
    (select ss_ticket_number
          ,ss_customer_sk
          ,ca_city bought_city
          ,sum(ss_coupon_amt) amt
          ,sum(ss_net_profit) profit
     from store_sales,date_dim,store,household_demographics,customer_address
     where store_sales.ss_sold_date_sk = date_dim.d_date_sk
       and store_sales.ss_store_sk = store.s_store_sk
       and store_sales.ss_hdemo_sk = household_demographics.hd_demo_sk
       and store_sales.ss_addr_sk = customer_address.ca_address_sk
       and (household_demographics.hd_dep_count = 8 or
            household_demographics.hd_vehicle_count= 4)
       and date_dim.d_dow in (6,0)
       and date_dim.d_year in (1999,1999+1,1999+2)
       and store.s_city in ('Five Points','Riverside','Fairview','Midway','Pleasant Hill')
     group by ss_ticket_number,ss_customer_sk,ss_addr_sk,ca_city) dn,customer,customer_address current_addr
where ss_customer_sk = c_customer_sk
  and customer.c_current_addr_sk = current_addr.ca_address_sk
  and current_addr.ca_city <> bought_city
order by c_last_name
       ,c_first_name
       ,ca_city
       ,bought_city
       ,ss_ticket_number
    limit 100;
-- 68
select  c_last_name
     ,c_first_name
     ,ca_city
     ,bought_city
     ,ss_ticket_number
     ,extended_price
     ,extended_tax
     ,list_price
from (select ss_ticket_number
           ,ss_customer_sk
           ,ca_city bought_city
           ,sum(ss_ext_sales_price) extended_price
           ,sum(ss_ext_list_price) list_price
           ,sum(ss_ext_tax) extended_tax
      from store_sales
         ,date_dim
         ,store
         ,household_demographics
         ,customer_address
      where store_sales.ss_sold_date_sk = date_dim.d_date_sk
        and store_sales.ss_store_sk = store.s_store_sk
        and store_sales.ss_hdemo_sk = household_demographics.hd_demo_sk
        and store_sales.ss_addr_sk = customer_address.ca_address_sk
        and date_dim.d_dom between 1 and 2
        and (household_demographics.hd_dep_count = 6 or
             household_demographics.hd_vehicle_count= 2)
        and date_dim.d_year in (2000,2000+1,2000+2)
        and store.s_city in ('Pleasant Hill','Oak Grove')
      group by ss_ticket_number
             ,ss_customer_sk
             ,ss_addr_sk,ca_city) dn
   ,customer
   ,customer_address current_addr
where ss_customer_sk = c_customer_sk
  and customer.c_current_addr_sk = current_addr.ca_address_sk
  and current_addr.ca_city <> bought_city
order by c_last_name
       ,ss_ticket_number
    limit 100;
--73
select c_last_name
     ,c_first_name
     ,c_salutation
     ,c_preferred_cust_flag
     ,ss_ticket_number
     ,cnt from
              (select ss_ticket_number
                    ,ss_customer_sk
                    ,count(*) cnt
               from store_sales,date_dim,store,household_demographics
               where store_sales.ss_sold_date_sk = date_dim.d_date_sk
                 and store_sales.ss_store_sk = store.s_store_sk
                 and store_sales.ss_hdemo_sk = household_demographics.hd_demo_sk
                 and date_dim.d_dom between 1 and 2
                 and (household_demographics.hd_buy_potential = '1001-5000' or
                      household_demographics.hd_buy_potential = '0-500')
                 and household_demographics.hd_vehicle_count > 0
                 and case when household_demographics.hd_vehicle_count > 0 then
                              household_demographics.hd_dep_count/ household_demographics.hd_vehicle_count else null end > 1
                 and date_dim.d_year in (2000,2000+1,2000+2)
                 and store.s_county in ('Walker County','Williamson County','Ziebach County','Walker County')
               group by ss_ticket_number,ss_customer_sk) dj,customer
where ss_customer_sk = c_customer_sk
  and cnt between 1 and 5
order by cnt desc, c_last_name asc;
-- 79
select
    c_last_name,c_first_name,substr(s_city,1,30),ss_ticket_number,amt,profit
from
    (select ss_ticket_number
          ,ss_customer_sk
          ,store.s_city
          ,sum(ss_coupon_amt) amt
          ,sum(ss_net_profit) profit
     from store_sales,date_dim,store,household_demographics
     where store_sales.ss_sold_date_sk = date_dim.d_date_sk
       and store_sales.ss_store_sk = store.s_store_sk
       and store_sales.ss_hdemo_sk = household_demographics.hd_demo_sk
       and (household_demographics.hd_dep_count = 9 or household_demographics.hd_vehicle_count > 3)
       and date_dim.d_dow = 1
       and date_dim.d_year in (1999,1999+1,1999+2)
       and store.s_number_employees between 200 and 295
     group by ss_ticket_number,ss_customer_sk,ss_addr_sk,store.s_city) ms,customer
where ss_customer_sk = c_customer_sk
order by c_last_name,c_first_name,substr(s_city,1,30), profit
    limit 100;
--Part of 23a
select substr(i_item_desc,1,30) itemdesc,i_item_sk item_sk,d_date solddate,count(*) cnt
from store_sales
   ,date_dim
   ,item
where ss_sold_date_sk = d_date_sk
  and ss_item_sk = i_item_sk
  and d_year in (1999,1999+1,1999+2,1999+3)
group by substr(i_item_desc,1,30),i_item_sk,d_date
having count(*) >4;
--23b
    with frequent_ss_items as
 (select substr(i_item_desc,1,30) itemdesc,i_item_sk item_sk,d_date solddate,count(*) cnt
  from store_sales
      ,date_dim
      ,item
  where ss_sold_date_sk = d_date_sk
    and ss_item_sk = i_item_sk
    and d_year in (1999,1999 + 1,1999 + 2,1999 + 3)
  group by substr(i_item_desc,1,30),i_item_sk,d_date
  having count(*) >4),
 max_store_sales as
 (select max(csales) tpcds_cmax
  from (select c_customer_sk,sum(ss_quantity*ss_sales_price) csales
        from store_sales
            ,customer
            ,date_dim
        where ss_customer_sk = c_customer_sk
         and ss_sold_date_sk = d_date_sk
         and d_year in (1999,1999+1,1999+2,1999+3)
        group by c_customer_sk) x),
 best_ss_customer as
 (select c_customer_sk,sum(ss_quantity*ss_sales_price) ssales
  from store_sales
      ,customer
  where ss_customer_sk = c_customer_sk
  group by c_customer_sk
  having sum(ss_quantity*ss_sales_price) > (95/100.0) * (select
  *
 from max_store_sales))
select  c_last_name,c_first_name,sales
from (select c_last_name,c_first_name,sum(cs_quantity*cs_list_price) sales
      from catalog_sales
         ,customer
         ,date_dim
      where d_year = 1999
        and d_moy = 7
        and cs_sold_date_sk = d_date_sk
        and cs_item_sk in (select item_sk from frequent_ss_items)
        and cs_bill_customer_sk in (select c_customer_sk from best_ss_customer)
        and cs_bill_customer_sk = c_customer_sk
      group by c_last_name,c_first_name
      union all
      select c_last_name,c_first_name,sum(ws_quantity*ws_list_price) sales
      from web_sales
         ,customer
         ,date_dim
      where d_year = 1999
        and d_moy = 7
        and ws_sold_date_sk = d_date_sk
        and ws_item_sk in (select item_sk from frequent_ss_items)
        and ws_bill_customer_sk in (select c_customer_sk from best_ss_customer)
        and ws_bill_customer_sk = c_customer_sk
      group by c_last_name,c_first_name) y
order by c_last_name,c_first_name,sales
    limit 100;
