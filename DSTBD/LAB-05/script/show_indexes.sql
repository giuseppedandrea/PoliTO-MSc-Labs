select USER_INDEXES.INDEX_NAME as INDEX_NAME, INDEX_TYPE, USER_INDEXES.TABLE_NAME, COLUMN_NAME||'('||COLUMN_POSITION||')' as COLUMN_NAME,
       BLEVEL, LEAF_BLOCKS, DISTINCT_KEYS, CLUSTERING_FACTOR
from user_indexes, user_ind_columns
where user_indexes.index_name=user_ind_columns.index_name and
      user_indexes.table_name=user_ind_columns.table_name;
